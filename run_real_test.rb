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
puts "Run real test"

run_command("rm -rf variant")
run_command("rm -rf target")


if (ARGV[0] != nil && ARGV[0] == "0a4ea19")

	puts "source/libssh/0a4ea19/pki.c"

	cfgs = configs("source/libssh/0a4ea19/pki.c")
	puts cfgs.size.to_s

	cid = 0
	for cfg in cfgs
		opts = config_opts(cfg)
		unless (opts.include?("_WIN32") || opts.include?("HAVE_LIBGCRYPT"))
			cid = cid + 1
			puts cid.to_s + " " + opts
			run_command("mkdir -p variant/libssh/0a4ea19_V#{cid}")
			puts" ===> " + command = "clang-3.5 -E #{config_opts(cfg)} -I source/libssh/0a4ea19/ -I source/libssh/0a4ea19/include_libgcrypt/ -I source/libssh/0a4ea19/include_libssh/ -I source/libssh/0a4ea19/include_openssl/ -o variant/libssh/0a4ea19_V#{cid}/pki.c source/libssh/0a4ea19/pki.c"
			if (run_command(command).include? "ERROR")
				puts "ERROR"
				puts run_command(command)
			else
				puts "PASS"
				puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -I source/libssh/0a4ea19/ -I source/libssh/0a4ea19/include_libgcrypt/ -I source/libssh/0a4ea19/include_libssh/ -I source/libssh/0a4ea19/include_openssl/ -o variant/libssh/0a4ea19_V#{cid}/pki.bc variant/libssh/0a4ea19_V#{cid}/pki.c"
				puts run_command(command)
				print "1 Time: " + command_median_repeat(command, 1).rjust(15, ' ') + " |"
				puts
				puts" ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_srv_pki_do_sign_sessionid variant/libssh/0a4ea19_V#{cid}/pki.bc"
				puts run_command(command)
				print "1 Time: " + command_median_repeat(command, 1).rjust(15, ' ') + " |"
				puts
			end
			puts
			puts
			puts
		end
	end


	puts "VARIANT  ------------------------------------------------"
	puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -I source/libssh/0a4ea19/ -I source/libssh/0a4ea19/include_libgcrypt/ -I source/libssh/0a4ea19/include_libssh/ -I source/libssh/0a4ea19/include_openssl/ -o variant/libssh/0a4ea19_V1/pki.bc variant/libssh/0a4ea19_V1/pki.c"
	puts "MEDIAN: " + command_median(command)
	puts
	puts" ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_srv_pki_do_sign_sessionid variant/libssh/0a4ea19_V1/pki.bc"
	puts "MEDIAN: " + command_median(command)
	puts

	puts "TARGET ------------------------------------------------"

	run_command("mkdir -p target/libssh/0a4ea19")
	puts" ===> " + command = "java -Xms2048m -Xmx10240m -Xss128m -jar reconfigurator.jar" +
		" -source #{Dir.pwd}/source/libssh/0a4ea19/pki.c" +
		" -target #{Dir.pwd}/target/libssh/0a4ea19/pki.c" +
		" -hdFile #{Dir.pwd}/source/libssh/0a4ea19/config.h" +
		" -include #{Dir.pwd}/source/libssh/0a4ea19/include_libssh/" +
		" -undef _WIN32" +
		" -undef _WIN64" +
		" -undef __MINGW32__" +
		" -undef __SUNPRO_C" +
		" -undef __CYGWIN__" +
		" -undef DES_RISC1" +
		" -undef OPENSSL_SYS_UEFI" +
		" -undef CRYPTO_TDEBUG" +
		" -undef __STDC_VERSION__" +
		" -define OPENSSL_THREADS" +
		" -define OPENSSL_NO_RSA" +
		" -define OPENSSL_NO_STDIO" +
		" -define OPENSSL_NO_SOCK" +
		" -define RECONFIGURATOR"
	puts run_command(command)
	puts

	puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall" +
		" -U _WIN32" +
		" -U _WIN64" +
		" -U __MINGW32__" +
		" -U __SUNPRO_C" +
		" -U __CYGWIN__" +
		" -U DES_RISC1" +
		" -U OPENSSL_SYS_UEFI" +
		" -U CRYPTO_TDEBUG" +
		" -U __STDC_VERSION__" +
		" -D OPENSSL_THREADS" +
		" -D OPENSSL_NO_RSA" +
		" -D OPENSSL_NO_STDIO" +
		" -D OPENSSL_NO_SOCK" +
		" -I source/libssh/0a4ea19/ -I source/libssh/0a4ea19/include_libgcrypt/ -I source/libssh/0a4ea19/include_libssh/ -I source/libssh/0a4ea19/include_openssl/ -o target/libssh/0a4ea19/pki.bc target/libssh/0a4ea19/pki.c"
	puts run_command(command)
	puts "MEDIAN: " + command_median(command)
	puts

	puts" ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_srv_pki_do_sign_sessionid target/libssh/0a4ea19/pki.bc"
	puts run_command(command)
	puts "MEDIAN: " + command_median(command)
	puts
	puts
	puts


	puts "BRUTE FORCE-------------------------------------------"
	puts
	puts" ===> " + command = (1..cid).map{ |i| "clang-3.5 -c -g -emit-llvm -Wall -o variant/libssh/0a4ea19_V#{i}/pki.bc variant/libssh/0a4ea19_V#{i}/pki.c"}.join(" ; ")
	puts "MEDIAN: " + command_median_repeat(command, 5).rjust(15, ' ') + " |"
	puts
	puts" ===> " + command = (1..cid).map{ |i| "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_srv_pki_do_sign_sessionid variant/libssh/0a4ea19_V#{i}/pki.bc"}.join(" ; ")
	puts "MEDIAN: " + command_median_repeat(command, 5).rjust(15, ' ') + " |"
	puts
















elsif (ARGV[0] != nil && ARGV[0] == "2a10019")
	puts "source/libssh/2a10019/sftp.c"

	cfgs = configs("source/libssh/2a10019/sftp.c")
	puts cfgs.size.to_s

	cid = 0
	for cfg in cfgs
		opts = config_opts(cfg)
		unless (opts.include?("_WIN32"))# || opts.include?("HAVE_LIBGCRYPT"))
			cid = cid + 1
			puts cid.to_s + " " + opts
			run_command("mkdir -p variant/libssh/2a10019_V#{cid}")
			puts" ===> " + command = "clang-3.5 -E #{config_opts(cfg)} -I source/libssh/2a10019/ -I source/libssh/2a10019/include_libssh/ -I source/libssh/2a10019/include_openssl/ -o variant/libssh/2a10019_V#{cid}/sftp.c source/libssh/2a10019/sftp.c"
			if (run_command(command).include? "ERROR")
				puts "ERROR"
				puts run_command(command)
			else
				puts "PASS"
				puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -I source/libssh/2a10019/ -I source/libssh/2a10019/include_libssh/ -I source/libssh/2a10019/include_openssl/ -o variant/libssh/2a10019_V#{cid}/sftp.bc source/libssh/2a10019/sftp.c"
				puts run_command(command)
				print "1 Time: " + command_median_repeat(command, 1).rjust(15, ' ') + " |"
				puts
			end
			puts
			puts
			puts
		end
	end


	puts "VARIANT  ------------------------------------------------"
	puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -I source/libssh/2a10019/ -I source/libssh/2a10019/include_libssh/ -I source/libssh/2a10019/include_openssl/ -o variant/libssh/2a10019_V1/sftp.bc source/libssh/2a10019/sftp.c"
	puts "MEDIAN: " + command_median(command)
	puts
	
	puts "TARGET ------------------------------------------------"

	run_command("mkdir -p target/libssh/2a10019")
	puts" ===> " + command = "java -Xms2048m -Xmx10240m -Xss128m -jar reconfigurator.jar" +
		" -source #{Dir.pwd}/source/libssh/2a10019/sftp.c" +
		" -target #{Dir.pwd}/target/libssh/2a10019/sftp.c" +
		" -hdFile #{Dir.pwd}/source/libssh/2a10019/config.h" +
		" -include #{Dir.pwd}/source/libssh/2a10019/include_libssh/" +
		" -undef _WIN32" +
		" -undef __CYGWIN__" +
		" -undef __SUNPRO_C" +
		""
	puts run_command(command)
	puts

	puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall" +
		" -U _WIN32" +
		" -U __CYGWIN__" +
		" -U __SUNPRO_C" +
		" -I source/libssh/2a10019/ -I source/libssh/2a10019/include_libssh/ -I source/libssh/2a10019/include_openssl/ -o target/libssh/2a10019/sftp.bc target/libssh/2a10019/sftp.c"
	puts run_command(command)
	puts "MEDIAN: " + command_median(command)
	puts

	puts "BRUTE FORCE-------------------------------------------"
	puts
	puts" ===> " + command = (1..cid).map{ |i| "clang-3.5 -c -g -emit-llvm -Wall -o variant/libssh/2a10019_V#{i}/sftp.bc variant/libssh/2a10019_V#{i}/sftp.c"}.join(" ; ")
	print "MEDIAN: " + command_median_repeat(command, 5).rjust(15, ' ') + " |"
















elsif (ARGV[0] != nil && ARGV[0] == "fadbe80")

	puts "source/libssh/fadbe80/options.c"

	cfgs = configs("source/libssh/fadbe80/options.c")
	puts cfgs.size.to_s

	cid = 0
	for cfg in cfgs
		opts = config_opts(cfg)
		unless (opts.include?("_WIN32"))# || opts.include?("HAVE_LIBGCRYPT"))
			cid = cid + 1
			puts cid.to_s + " " + opts
			run_command("mkdir -p variant/libssh/fadbe80_V#{cid}")
			puts" ===> " + command = "clang-3.5 -E #{config_opts(cfg)} -I source/libssh/fadbe80/ -I source/libssh/fadbe80/include_libgcrypt/ -I source/libssh/fadbe80/include_libssh/ -I source/libssh/fadbe80/include_openssl/ -o variant/libssh/fadbe80_V#{cid}/options.c source/libssh/fadbe80/options.c"
			if (run_command(command).include? "ERROR")
				puts "ERROR"
				puts run_command(command)
			else
				puts "PASS"
				puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall -I source/libssh/fadbe80/ -I source/libssh/fadbe80/include_libgcrypt/ -I source/libssh/fadbe80/include_libssh/ -I source/libssh/fadbe80/include_openssl/ -o variant/libssh/fadbe80_V#{cid}/options.bc variant/libssh/fadbe80_V#{cid}/options.c"
				puts run_command(command)
				print "1 Time: " + command_median_repeat(command, 1).rjust(15, ' ') + " |"
				puts
				puts" ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_options_getopt variant/libssh/fadbe80_V#{cid}/options.bc"
				puts run_command(command)
				print "1 Time: " + command_median_repeat(command, 1).rjust(15, ' ') + " |"
				puts
			end
			puts
			puts
			puts
		end
	end


	puts "VARIANT  ------------------------------------------------"
	puts" ===> " + command = "clang-3.5 -E -D WITH_SERVER -D WITH_SSH1 -I source/libssh/fadbe80/ -I source/libssh/fadbe80/include_libgcrypt/ -I source/libssh/fadbe80/include_libssh/ -I source/libssh/fadbe80/include_openssl/ -o variant/libssh/fadbe80_V5/options.c source/libssh/fadbe80/options.c"
	puts "MEDIAN: " + command_median(command)
	puts
	puts" ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_options_getopt variant/libssh/fadbe80_V5/options.bc"
	puts "MEDIAN: " + command_median(command)
	puts

	puts "TARGET ------------------------------------------------"

	# run_command("mkdir -p target/libssh/fadbe80")
	# puts" ===> " + command = "java -Xms2048m -Xmx10240m -Xss128m -jar reconfigurator.jar" +
	# 	" -source #{Dir.pwd}/source/libssh/fadbe80/options.c" +
	# 	" -target #{Dir.pwd}/target/libssh/fadbe80/options.c" +
	# 	" -hdFile #{Dir.pwd}/source/libssh/fadbe80/config.h" +
	# 	" -include #{Dir.pwd}/source/libssh/fadbe80/include_libssh/" +
	# # 	" -undef _WIN32" +
	# # 	" -undef _WIN64" +
	# # 	" -undef __MINGW32__" +
	# # 	" -undef __SUNPRO_C" +
	# # 	" -undef __CYGWIN__" +
	# # 	" -undef DES_RISC1" +
	# # 	" -undef OPENSSL_SYS_UEFI" +
	# # 	" -undef CRYPTO_TDEBUG" +
	# # 	" -undef __STDC_VERSION__" +
	# # 	" -define OPENSSL_THREADS" +
	# # 	" -define OPENSSL_NO_RSA" +
	# # 	" -define OPENSSL_NO_STDIO" +
	# # 	" -define OPENSSL_NO_SOCK" +
	# 	" -define RECONFIGURATOR"
	# puts run_command(command)
	# puts

	# # puts" ===> " + command = "clang-3.5 -c -g -emit-llvm -Wall" +
	# # 	" -U _WIN32" +
	# # 	" -U _WIN64" +
	# # 	" -U __MINGW32__" +
	# # 	" -U __SUNPRO_C" +
	# # 	" -U __CYGWIN__" +
	# # 	" -U DES_RISC1" +
	# # 	" -U OPENSSL_SYS_UEFI" +
	# # 	" -U CRYPTO_TDEBUG" +
	# # 	" -U __STDC_VERSION__" +
	# # 	" -D OPENSSL_THREADS" +
	# # 	" -D OPENSSL_NO_RSA" +
	# # 	" -D OPENSSL_NO_STDIO" +
	# # 	" -D OPENSSL_NO_SOCK" +
	# # 	" -I source/libssh/0a4ea19/ -I source/libssh/0a4ea19/include_libgcrypt/ -I source/libssh/0a4ea19/include_libssh/ -I source/libssh/0a4ea19/include_openssl/ -o target/libssh/0a4ea19/pki.bc target/libssh/0a4ea19/pki.c"
	# # puts run_command(command)
	# # puts "MEDIAN: " + command_median(command)
	# # puts

	puts " ===> " + command = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_options_getopt oracle/libssh/fadbe80/options.bc"
	puts run_command(command)
	puts "MEDIAN: " + command_median(command)
	puts
	puts
	puts


	puts "BRUTE FORCE-------------------------------------------"
	puts
	puts " ===> " + command = (1..cid).map{ |i| "clang-3.5 -c -g -emit-llvm -Wall -o variant/libssh/fadbe80_V#{i}/options.bc variant/libssh/fadbe80_V#{i}/options.c"}.join(" ; ")
	puts "MEDIAN: " + command_median_repeat(command, 5).rjust(15, ' ') + " |"
	puts
	puts " ===> " + command = (1..cid).map{ |i| "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks --ignore-undetermined-functions -function-name=ssh_options_getopt variant/libssh/fadbe80_V#{i}/options.bc"}.join(" ; ")
	puts "MEDIAN: " + command_median_repeat(command, 5).rjust(15, ' ') + " |"

end

puts "End test"