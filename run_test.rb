require 'open3'

def time_diff_milli(start, finish)
   #((finish - start) * 1000.0).round(2)
   (finish - start) * 1000
end

def median(array)
  sorted = array.sort
  len = sorted.length
  (sorted[(len - 1) / 2] + sorted[len / 2]) / 2.0
end

def command_median_repeat (command, repeat)
	status = 0
	times = Array.new 
	(1..repeat).each do |n|
		t1 = Time.now
		stdout,stderr,status = Open3.capture3(command)
		t2 = Time.now
		msecs = time_diff_milli t1, t2
		times << msecs
	end

	sprintf '%.0f', median(times)
end

def command_median (command)
	command_median_repeat(command, 50)
end

def run_command (command)
	stdout,stderr,status = Open3.capture3(command)
	output = stdout
	if (stderr.length != 0)
		output = output + "\nERROR:\n" + stderr
	end
	return output
end


@files_H = Hash.new("")
@files_H["76baeeb"] = "vbdb/linux/"
@files_H["f7ab9b4"] = "vbdb/linux/"
@files_H["ee3f34e"] = "vbdb/linux/"
@files_H["6252547"] = "vbdb/linux/"
@files_H["8c82962"] = "vbdb/linux/"
@files_H["60e233a"] = "vbdb/linux/"
@files_H["0f8f809"] = "vbdb/linux/"
@files_H["7acf6cd"] = "vbdb/linux/"
@files_H["bc8cec0"] = "vbdb/linux/"
@files_H["30e0532"] = "vbdb/linux/"
@files_H["1c17e4d"] = "vbdb/linux/"
@files_H["e39363a"] = "vbdb/linux/"
@files_H["7c6048b"] = "vbdb/linux/"
@files_H["2f02c15"] = "vbdb/linux/"
@files_H["6515e48"] = "vbdb/linux/"
@files_H["242f1a3"] = "vbdb/linux/"
@files_H["6651791"] = "vbdb/linux/"
@files_H["f48ec1d"] = "vbdb/linux/"
@files_H["e67bc51"] = "vbdb/linux/"
@files_H["e68bb91"] = "vbdb/linux/"
@files_H["809e660"] = "vbdb/linux/"
@files_H["d6c7e11"] = "vbdb/linux/"
@files_H["63878ac"] = "vbdb/linux/"
@files_H["657e964"] = "vbdb/linux/"
@files_H["0988c4c"] = "vbdb/linux/"

@files_H["199501f"] = "vbdb/busybox/"
@files_H["1b487ea"] = "vbdb/busybox/"
@files_H["b273d66"] = "vbdb/busybox/"
@files_H["cf1f2ac"] = "vbdb/busybox/"
@files_H["ebee301"] = "vbdb/busybox/"
@files_H["5275b1e"] = "vbdb/busybox/"
@files_H["b7ebc61"] = "vbdb/busybox/"
@files_H["5cd6461"] = "vbdb/busybox/"

#@files_H["0a4ea19"] = "libssh/"



@real_name_H = Hash.new("")
#@real_name_H["0a4ea19"] = "pki"



@variant_config_H = Hash.new("")
@variant_config_H["76baeeb"] = " -D CONFIG_X86_32 -D CONFIG_NUMA -D CONFIG_PCI"
@variant_config_H["f7ab9b4"] = " -D CONFIG_DRM_I915 -D CONFIG_SHMEM -U CONFIG_TMPFS"
@variant_config_H["ee3f34e"] = " -D CONFIG_IP_SCTP -D CONFIG_IPV6 -D CONFIG_SCTP_DBG_MSG"
@variant_config_H["6252547"] = " -D CONFIG_TWL4030_CORE -U CONFIG_OF_IRQ"
@variant_config_H["8c82962"] = " -D CONFIG_PROC_PAGE_MONITOR"
@variant_config_H["60e233a"] = " -U CONFIG_HOTPLUG"
@variant_config_H["0f8f809"] = " -D CONFIG_LOCKDEP -D CONFIG_SLAB -U CONFIG_SLOB -D CONFIG_PPC_256K_PAGES"
@variant_config_H["7acf6cd"] = " -D CONFIG_UNIX98_PTYS -D CONFIG_DEVPTS_MULTIPLE_INSTANCES"
@variant_config_H["bc8cec0"] = " -D CONFIG_JFFS2_FS_WBUF_VERIFY"
@variant_config_H["30e0532"] = " -U CONFIG_SECURITY"
@variant_config_H["1c17e4d"] = " -D CONFIG_TREE_RCU -D CONFIG_RCU_CPU_STALL_INFO -U CONFIG_RCU_FAST_NO_HZ"
@variant_config_H["e39363a"] = " -D CONFIG_NETPOLL -U CONFIG_IPV6"
@variant_config_H["7c6048b"] = " -D CONFIG_STUB_POULSBO -D CONFIG_ACPI_VIDEO -U CONFIG_BACKLIGHT_CLASS_DEVICE"
@variant_config_H["2f02c15"] = " -D CONFIG_BCM47XX -U CONFIG_SSB_DRIVER_EXTIF"
@variant_config_H["6515e48"] = " -D CONFIG_MACH_OMAP_H4 -U CONFIG_ARCH_OMAP2420"
@variant_config_H["242f1a3"] = " -D CONFIG_CRYPTO_TEST -U CONFIG_CRYPTO_BLKCIPHER"
@variant_config_H["6651791"] = " -D CONFIG_SND_FSI_AK4642 -D CONFIG_SND_FSI_DA7210 -U CONFIG_I2C"
@variant_config_H["f48ec1d"] = " -D CONFIG_IOSCHED_CFQ -U CONFIG_BLK_CGROUP"
@variant_config_H["e67bc51"] = " -U CONFIG_TRACING"
@variant_config_H["e68bb91"] = " -D CONFIG_I2C_DESIGNWARE_PLATFORM -U CONFIG_I2C_DESIGNWARE_PCI"
@variant_config_H["809e660"] = " -D CONFIG_ARM_LPAE"
@variant_config_H["d6c7e11"] = " -D CONFIG_REGULATOR_MAX8660 -U CONFIG_OF"
@variant_config_H["63878ac"] = " -D CONFIG_ARCH_OMAP3 -U CONFIG_PM"
@variant_config_H["657e964"] = " -D CONFIG_TCP_MD5SIG -D CONFIG_PREEMPT"
@variant_config_H["0988c4c"] = " -U CONFIG_VLAN_8021Q"

@variant_config_H["199501f"] = " -U ENABLE_FEATURE_MODPROBE_MULTIPLE_OPTIONS"
@variant_config_H["1b487ea"] = " -D ENABLE_SELINUX -U ENABLE_FEATURE_STAT_FORMAT"
@variant_config_H["b273d66"] = " -D BB_FEATURE_LS_FILETYPES -U BB_FEATURE_LS_USERNAME"
@variant_config_H["cf1f2ac"] = " -D ENABLE_DEBUG_INIT"
@variant_config_H["ebee301"] = " -D BB_CP_MV -U BB_AR"
@variant_config_H["5275b1e"] = " -U CONFIG_FEATURE_INSMOD_VERSION_CHECKING"
@variant_config_H["b7ebc61"] = " -U CONFIG_FEATURE_LS_TIMESTAMPS"
@variant_config_H["5cd6461"] = " -D CONFIG_LFS -D CONFIG_FEATURE_HTTPD_BASIC_AUTH -D CONFIG_FEATURE_HTTPD_CGI"

@llbmc_args_H = Hash.new("")
@llbmc_args_H["76baeeb"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["f7ab9b4"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["ee3f34e"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["6252547"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["8c82962"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["60e233a"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["0f8f809"] = "-ignore-missing-function-bodies --max-loop-iterations=100 --ignore-undetermined-functions"
@llbmc_args_H["7acf6cd"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["bc8cec0"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["30e0532"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["1c17e4d"] = "-ignore-missing-function-bodies --max-loop-iterations=100 --ignore-undetermined-functions"
@llbmc_args_H["e39363a"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["7c6048b"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["2f02c15"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["6515e48"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["242f1a3"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["6651791"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["f48ec1d"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["e67bc51"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["e68bb91"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["809e660"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["d6c7e11"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["63878ac"] = "-no-custom-assertions -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["657e964"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["0988c4c"] = "-ignore-missing-function-bodies --max-loop-iterations=100 --ignore-undetermined-functions"

@llbmc_args_H["199501f"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["1b487ea"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["b273d66"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["cf1f2ac"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["ebee301"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["5275b1e"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["b7ebc61"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"
@llbmc_args_H["5cd6461"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions"

# OTHER LLBMC ARGS
# 	llbmc_H["0dc77b6"] = "llbmc --max-loop-iterations=100 0dc77b6/variant-err/0dc77b6.bc"
# 	llbmc_H["1f758a4"] = "llbmc --max-loop-iterations=100 1f758a4/variant-err/1f758a4.bc"
# 	llbmc_H["208d898"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 208d898/variant-err/208d898.bc"
# 	llbmc_H["218ad12"] = "llbmc -leak-check --max-loop-iterations=100 218ad12/variant-err/218ad12.bc"

def folder (folder, file, ext)
	if (@real_name_H[file].length == 0)
		"#{folder}/#{@files_H[file]}#{file}/#{file}#{ext}"
	else
		"#{folder}/#{@files_H[file]}#{file}/#{@real_name_H[file]}#{ext}"
	end
end

def source (file)
	folder("source", file, ".c")
end

def target (file)
	folder("target", file, ".c")
end

def variant (file)
	folder("variant", file, ".c")
end

def oracle (file)
	folder("oracle", file, ".c")
end

def targetBC (file)
	folder("target", file, ".bc")
end

def variantBC (file)
	folder("variant", file, ".bc")
end

def oracleBC (file)
	folder("oracle", file, ".bc")
end


def configs(file)
  name  = /(?<macro>[A-Z0-9_]+)/
  regex = Regexp.union(/#if(n)?def #{name}/, /#(el)?if defined( )?\(#{name}\)/)
  opts  = File.open(file).read.scan(regex).flatten.compact.uniq
  [[]] + (1 .. opts.length).map { |i| opts.combination(i).to_a }.flatten(1)
end


def config_opts(config)
  if config == []
    ""
  else
    "-D " + config.join(" -D ")
  end
end

puts "\n"*20
puts "Run test"

run_command("rm -rf variant")
run_command("rm -rf target")

if (ARGV[0] != nil && ARGV[0] == "-bf")
	puts "running brute force"

	id = 0
	puts " ID  | HASH    | no. configs | frama-c -bf (ms) | clang -bf (ms) | llbmc -bf (ms) |"
	puts "-----------------------------------------------------------------------------------"
	for file in @files_H.keys
		id = id + 1
		print id.to_s.rjust(4, ' ') + " |"
		print file.rjust(8, ' ') + " |"
		
		cfgs = configs(source(file))
		print cfgs.size.to_s.rjust(12, ' ') + " |"
		

		cid = 0
		for cfg in cfgs
			cid = cid + 1
			cid.to_s + " " + config_opts(cfg)
			run_command("mkdir -p variant/#{@files_H[file]}#{file}_V#{cid}")
			" ===> " + command = "clang-3.5 -E #{config_opts(cfg)} -I source/#{@files_H[file]}#{file} -o variant/#{@files_H[file]}#{file}_V#{cid}/#{file}.c #{source(file)}"
			"\n" + run_command(command)
		end

		" ===> " + command = (1..cid).map{ |i| "frama-c -val -quiet variant/#{@files_H[file]}#{file}_V#{i}/#{file}.c"}.join(" ; ")
		print command_median_repeat(command, 5).rjust(17, ' ') + " |"
		
		" ===> " + command = (1..cid).map{ |i| "clang-3.5 -c -g -emit-llvm -Wall -o variant/#{@files_H[file]}#{file}_V#{i}/#{file}.bc variant/#{@files_H[file]}#{file}_V#{i}/#{file}.c"}.join(" ; ")
		print command_median_repeat(command, 5).rjust(15, ' ') + " |"
		
		" ===> " + command = (1..cid).map{ |i| "llbmc #{@llbmc_args_H[file]} variant/#{@files_H[file]}#{file}_V#{i}/#{file}.bc"}.join(" ; ")
		print command_median_repeat(command, 5).rjust(15, ' ') + " |"
		
		puts
	end
elsif (ARGV[0] != nil)
	if (@files_H.keys.include?(ARGV[0]))
		file = ARGV[0]
		puts
		puts "--------------------------------------------------------------"
		puts "  TESTING " + file
		puts
		puts "--------------------------------------------------------------"
		puts "  C-RECONFIGURATOR"
		puts
		run_command("mkdir -p variant/#{@files_H[file]}#{file}")
		puts " ===> " + command = "clang-3.5 -E#{@variant_config_H[file]} -I source/#{@files_H[file]}#{file} -o #{variant(file)} #{source(file)}"
		puts "\n" + run_command(command)
		puts
		run_command("mkdir -p target/#{@files_H[file]}#{file}")
		puts " ===> " + command = "java -Xms2048m -Xmx10240m -Xss128m -jar reconfigurator.jar -source #{Dir.pwd}/#{source(file)} -target #{Dir.pwd}/#{target(file)} -oracle #{Dir.pwd}/#{oracle(file)} -include #{Dir.pwd}/source/#{@files_H[file]}#{file}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  SIZES"
		puts
		puts "  source size: " + run_command("stat --printf=\"%s\" #{source(file)}") + "B"
		puts "  target size: " + run_command("stat --printf=\"%s\" #{target(file)}") + "B"
		puts
		puts "--------------------------------------------------------------"
		puts "  FRAMA-C"
		puts
		puts " ===> " + command = "frama-c -val -quiet #{variant(file)}"
		puts "\n" + run_command(command)
		puts
		puts " ===> " + command = "frama-c -val -quiet #{target(file)}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  CLANG"
		puts
		puts " ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -o #{variantBC(file)} #{variant(file)}"
		puts "\n" + run_command(command)
		puts
		puts " ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -o #{targetBC(file)} #{target(file)}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  LLBMC"
		puts
		puts " ===> " + command = "llbmc #{@llbmc_args_H[file]} #{variantBC(file)}"
		puts "\n" + run_command(command)
		puts
		puts " ===> " + command = "llbmc #{@llbmc_args_H[file]} #{targetBC(file)}"
		puts "\n" + run_command(command)
	else
		puts "file not found"
	end
else
	id = 0
	puts " ID  | HASH    | file size (B)   | frama-c (ms)    | clang (ms)      | llbmc (ms)      |"
	puts "     |         | source | target | var    | target | var    | target | var    | target |"
	puts "----------------------------------------------------------------------------------------"
	for file in @files_H.keys
		id = id + 1
		print id.to_s.rjust(4, ' ') + " |"
		print file.rjust(8, ' ') + " |"
		
		run_command("mkdir -p variant/#{@files_H[file]}#{file}")
		run_command("clang-3.5 -E#{@variant_config_H[file]} -I source/#{@files_H[file]}#{file} -o #{variant(file)} #{source(file)}")

		run_command("mkdir -p target/#{@files_H[file]}#{file}")
		run_command("java -Xms2048m -Xmx10240m -Xss128m -jar reconfigurator.jar -source #{Dir.pwd}/#{source(file)} -target #{Dir.pwd}/#{target(file)} -oracle #{Dir.pwd}/#{oracle(file)} -I source/#{@files_H[file]}#{file}")

		print run_command("stat --printf=\"%s\" #{source(file)}").rjust(7, ' ') + " |"
		print run_command("stat --printf=\"%s\" #{target(file)}").rjust(7, ' ') + " |"

		print command_median("frama-c -val -quiet #{variant(file)}").rjust(7, ' ') + " |"
		print command_median("frama-c -val -quiet #{target(file)}").rjust(7, ' ') + " |"
		
		print command_median("clang-3.5 -c -g -emit-llvm -Wall -o #{variantBC(file)} #{variant(file)}").rjust(7, ' ') + " |"
		print command_median("clang-3.5 -c -g -emit-llvm -Wall -o #{targetBC(file)} #{target(file)}").rjust(7, ' ') + " |"

		print command_median("llbmc #{@llbmc_args_H[file]} #{variantBC(file)}").rjust(7, ' ') + " |"
		print command_median("llbmc #{@llbmc_args_H[file]} #{targetBC(file)}").rjust(7, ' ') + " |"

		puts
	end
end

puts "End test"