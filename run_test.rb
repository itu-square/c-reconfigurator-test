require 'open3'

def time_diff_milli(start, finish)
   #((finish - start) * 1000.0).round(2)
   (finish - start).round(3)
end

def median(array)
  sorted = array.sort
  len = sorted.length
  (sorted[(len - 1) / 2] + sorted[len / 2]) / 2.0
end

def command_median (id, command)
	status = ""
	times = Array.new 
	(1..10).each do |n|
		t1 = Time.now
		stdout,stderr,status = Open3.capture3(command)
		t2 = Time.now
		msecs = time_diff_milli t1, t2
		times << msecs
	end
	puts id + (sprintf '%.3f', median(times)) + " ms :> " + command
	#puts sprintf '%.2f', msecs

	if (status != 0)
		puts ":> ERROR"
	end
end

def run_command (command)
	stdout,stderr,status = Open3.capture3(command)
	stdout
end


@files_H = Hash.new("unknown")
@files_H["76baeeb"] = "vbdb/linux/"

def folder (folder, file)
	"#{folder}/#{@files_H[file]}#{file}/#{file}.c"
end

def source (file)
	folder("source", file)
end

def target (file)
	folder("target", file)
end

def oracle (file)
	folder("oracle", file)
end

puts "\n"*20
puts "Run test"

if (ARGV[0] == "size")
	puts "HASH       | source | oracle | "
	puts "-------------------------------"
	for file in @files_H.keys
		puts file.ljust(10, ' ') + " | " +
			run_command("stat stat --printf=\"%s\" #{source(file)}").ljust(6, ' ') + " | " +
			run_command("stat stat --printf=\"%s\" #{oracle(file)}").ljust(6, ' ') + " | "
	end
else
	for file in @files_H.keys
		puts "source/#{@files_H[file]}#{file}/#{file}.c"
		puts source(file)
		puts target(file)
		puts oracle(file)
	end
end

# for file in Dir.glob("./simple/*.c").sort
# 	puts file

# 	name = File.basename(file, ".c")
# 	# Dir.mkdir name
# 	# Dir.mkdir name + "/variant-err"
# 	# Dir.mkdir name + "/source-err"
# 	# Dir.mkdir name + "/target-err"
# 	# system "cp simple/#{name}.c #{name}/source-err/"
# 	# system "cp simple-target/#{name}.c #{name}/target-err/"

# 	# command = "clang -E -include varconfig.h #{name}/source-err/#{name}.c -o #{name}/variant-err/#{name}.c"
# 	# stdout,stderr,status = Open3.capture3(command)
# 	# puts ":> " + command
# 	# if (status != 0)
# 	# 	puts ":> ERROR"
# 	# end

# 	command_median("frama variant: ", "frama-c -val #{name}/variant-err/#{name}.c")
	
# 	command_median("frama reconfd: ", "frama-c -val #{name}/target-err/#{name}.c")

	
# 	command_median("clang variant: ", "clang -c -g -emit-llvm -Wall -include varconfig.h #{name}/source-err/#{name}.c -o #{name}/variant-err/#{name}.bc")

# 	command_median("clang reconfd: ", "clang -c -g -emit-llvm -Wall -include recconfig.h #{name}/target-err/#{name}.c -o #{name}/target-err/#{name}.bc")

	

# 	llbmc_H = Hash.new("llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks [&NAME]/variant-err/[&NAME].bc")
# 	llbmc_H["0988c4c"] = "llbmc --max-loop-iterations=100 0988c4c/variant-err/0988c4c.bc"
# 	llbmc_H["0dc77b6"] = "llbmc --max-loop-iterations=100 0dc77b6/variant-err/0dc77b6.bc"
# 	llbmc_H["0f8f809"] = "llbmc --max-loop-iterations=100 0f8f809/variant-err/0f8f809.bc"
# 	llbmc_H["1c17e4d"] = "llbmc --ignore-missing-function-bodies --max-loop-iterations=100 1c17e4d/variant-err/1c17e4d.bc"
# 	llbmc_H["1f758a4"] = "llbmc --max-loop-iterations=100 1f758a4/variant-err/1f758a4.bc"
# 	llbmc_H["208d898"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 208d898/variant-err/208d898.bc"
# 	llbmc_H["218ad12"] = "llbmc -leak-check --max-loop-iterations=100 218ad12/variant-err/218ad12.bc"
# 	llbmc_H["221ac32"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 221ac32/variant-err/221ac32.bc"
# 	llbmc_H["30e0532"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 30e0532/variant-err/30e0532.bc"
# 	llbmc_H["36855dc"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 36855dc/variant-err/36855dc.bc"
# 	llbmc_H["472a474"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 472a474/variant-err/472a474.bc"
# 	llbmc_H["60e233a"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 60e233a/variant-err/60e233a.bc"
# 	llbmc_H["6252547"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 6252547/variant-err/6252547.bc"
# 	llbmc_H["63878ac"] = "llbmc -no-custom-assertions -ignore-missing-function-bodies --no-max-loop-iterations-checks 63878ac/variant-err/63878ac.bc"
# 	llbmc_H["657e964"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 657e964/variant-err/657e964.bc"
# 	llbmc_H["76baeeb"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 76baeeb/variant-err/76baeeb.bc"
# 	llbmc_H["7acf6cd"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 7acf6cd/variant-err/7acf6cd.bc"
# 	llbmc_H["8c82962"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 8c82962/variant-err/8c82962.bc"
# 	llbmc_H["91ea820"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 91ea820/variant-err/91ea820.bc"
# 	llbmc_H["ae249b5"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks ae249b5/variant-err/ae249b5.bc"
# 	llbmc_H["eb91f1d"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks eb91f1d/variant-err/eb91f1d.bc"
# 	llbmc_H["f3d83e2"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks f3d83e2/variant-err/f3d83e2.bc"
# 	llbmc_H["f7ab9b4"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks f7ab9b4/variant-err/f7ab9b4.bc"



# 	command_median("llbmc variant: ", llbmc_H[name].gsub("[&NAME]", name))

	

# 	llbmc_H = Hash.new("llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks [&NAME]/target-err/[&NAME].bc")
# 	llbmc_H["0988c4c"] = "llbmc --max-loop-iterations=100 0988c4c/target-err/0988c4c.bc"
# 	llbmc_H["0dc77b6"] = "llbmc --max-loop-iterations=100 0dc77b6/target-err/0dc77b6.bc"
# 	llbmc_H["0f8f809"] = "llbmc --max-loop-iterations=100 0f8f809/target-err/0f8f809.bc"
# 	llbmc_H["1c17e4d"] = "llbmc --ignore-missing-function-bodies --max-loop-iterations=100 1c17e4d/target-err/1c17e4d.bc"
# 	llbmc_H["1f758a4"] = "llbmc --max-loop-iterations=100 1f758a4/target-err/1f758a4.bc"
# 	llbmc_H["208d898"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 208d898/target-err/208d898.bc"
# 	llbmc_H["218ad12"] = "llbmc -leak-check --max-loop-iterations=100 218ad12/target-err/218ad12.bc"
# 	llbmc_H["221ac32"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 221ac32/target-err/221ac32.bc"
# 	llbmc_H["30e0532"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 30e0532/target-err/30e0532.bc"
# 	llbmc_H["36855dc"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 36855dc/target-err/36855dc.bc"
# 	llbmc_H["472a474"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 472a474/target-err/472a474.bc"
# 	llbmc_H["60e233a"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 60e233a/target-err/60e233a.bc"
# 	llbmc_H["6252547"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 6252547/target-err/6252547.bc"
# 	llbmc_H["63878ac"] = "llbmc -no-custom-assertions -ignore-missing-function-bodies --no-max-loop-iterations-checks 63878ac/target-err/63878ac.bc"
# 	llbmc_H["657e964"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 657e964/target-err/657e964.bc"
# 	llbmc_H["76baeeb"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 76baeeb/target-err/76baeeb.bc"
# 	llbmc_H["7acf6cd"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 7acf6cd/target-err/7acf6cd.bc"
# 	llbmc_H["8c82962"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 8c82962/target-err/8c82962.bc"
# 	llbmc_H["91ea820"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 91ea820/target-err/91ea820.bc"
# 	llbmc_H["ae249b5"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks ae249b5/target-err/ae249b5.bc"
# 	llbmc_H["eb91f1d"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks eb91f1d/target-err/eb91f1d.bc"
# 	llbmc_H["f7ab9b4"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks f7ab9b4/target-err/f7ab9b4.bc"

# 	command_median("llbmc reconfd: ", llbmc_H[name].gsub("[&NAME]", name))

# 	puts
# end

puts "End test"