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

def command_median (command)
	status = 0
	times = Array.new 
	(1..50).each do |n|
		t1 = Time.now
		stdout,stderr,status = Open3.capture3(command)
		t2 = Time.now
		msecs = time_diff_milli t1, t2
		times << msecs
	end

	sprintf '%.0f', median(times)
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

@variant_config_H = Hash.new("")
@variant_config_H["76baeeb"] = "-D CONFIG_X86_32 -D CONFIG_NUMA -D CONFIG_PCI"
@variant_config_H["f7ab9b4"] = "-D CONFIG_DRM_I915 -D CONFIG_SHMEM -U CONFIG_TMPFS"
@variant_config_H["ee3f34e"] = "-D CONFIG_IP_SCTP -D CONFIG_IPV6 -D CONFIG_SCTP_DBG_MSG"
@variant_config_H["6252547"] = "-D CONFIG_TWL4030_CORE -U CONFIG_OF_IRQ"
@variant_config_H["8c82962"] = "-D CONFIG_PROC_PAGE_MONITOR"
@variant_config_H["60e233a"] = "-U CONFIG_HOTPLUG"
@variant_config_H["0f8f809"] = "-D CONFIG_LOCKDEP -D CONFIG_SLAB -U CONFIG_SLOB -D CONFIG_PPC_256K_PAGES"
@variant_config_H["7acf6cd"] = "-D CONFIG_UNIX98_PTYS -D CONFIG_DEVPTS_MULTIPLE_INSTANCES"
@variant_config_H["bc8cec0"] = "-D CONFIG_JFFS2_FS_WBUF_VERIFY"
@variant_config_H["30e0532"] = "-U CONFIG_SECURITY"
@variant_config_H["1c17e4d"] = "-D CONFIG_TREE_RCU -D CONFIG_RCU_CPU_STALL_INFO -U CONFIG_RCU_FAST_NO_HZ"
@variant_config_H["e39363a"] = "-D CONFIG_NETPOLL -U CONFIG_IPV6"
@variant_config_H["7c6048b"] = "-D CONFIG_STUB_POULSBO -D CONFIG_ACPI_VIDEO -U CONFIG_BACKLIGHT_CLASS_DEVICE"
@variant_config_H["2f02c15"] = "-D CONFIG_BCM47XX -U CONFIG_SSB_DRIVER_EXTIF"
@variant_config_H["6515e48"] = "-D CONFIG_MACH_OMAP_H4 -U CONFIG_ARCH_OMAP2420"
@variant_config_H["242f1a3"] = "-D CONFIG_CRYPTO_TEST -U CONFIG_CRYPTO_BLKCIPHER"
@variant_config_H["6651791"] = "-D CONFIG_SND_FSI_AK4642 -D CONFIG_SND_FSI_DA7210 -U CONFIG_I2C"
@variant_config_H["f48ec1d"] = "-D CONFIG_IOSCHED_CFQ -U CONFIG_BLK_CGROUP"
@variant_config_H["e67bc51"] = "-U CONFIG_TRACING"
@variant_config_H["e68bb91"] = "-D CONFIG_I2C_DESIGNWARE_PLATFORM -U CONFIG_I2C_DESIGNWARE_PCI"
@variant_config_H["809e660"] = "-D CONFIG_ARM_LPAE"
@variant_config_H["d6c7e11"] = "-D CONFIG_REGULATOR_MAX8660 -U CONFIG_OF"
@variant_config_H["63878ac"] = "-D CONFIG_ARCH_OMAP3 -U CONFIG_PM"
@variant_config_H["657e964"] = "-D CONFIG_TCP_MD5SIG -D CONFIG_PREEMPT"
@variant_config_H["0988c4c"] = "-U CONFIG_VLAN_8021Q"

@variant_config_H["199501f"] = "-U ENABLE_FEATURE_MODPROBE_MULTIPLE_OPTIONS"
@variant_config_H["1b487ea"] = "-D ENABLE_SELINUX -U ENABLE_FEATURE_STAT_FORMAT"
@variant_config_H["b273d66"] = "-D BB_FEATURE_LS_FILETYPES -U BB_FEATURE_LS_USERNAME"
@variant_config_H["cf1f2ac"] = "-D ENABLE_DEBUG_INIT"
@variant_config_H["ebee301"] = "-D BB_CP_MV -U BB_AR"
@variant_config_H["5275b1e"] = "-U CONFIG_FEATURE_INSMOD_VERSION_CHECKING"
@variant_config_H["b7ebc61"] = "-U CONFIG_FEATURE_LS_TIMESTAMPS"
@variant_config_H["5cd6461"] = "-D CONFIG_LFS -D CONFIG_FEATURE_HTTPD_BASIC_AUTH -D CONFIG_FEATURE_HTTPD_CGI"

@llbmc_args_H = Hash.new("")
@llbmc_args_H["76baeeb"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["f7ab9b4"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["ee3f34e"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["6252547"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["8c82962"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["60e233a"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["0f8f809"] = "--max-loop-iterations=100"
@llbmc_args_H["7acf6cd"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["bc8cec0"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["30e0532"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["1c17e4d"] = "-ignore-missing-function-bodies --max-loop-iterations=100"
@llbmc_args_H["e39363a"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["7c6048b"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["2f02c15"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["6515e48"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["242f1a3"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["6651791"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["f48ec1d"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["e67bc51"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["e68bb91"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["809e660"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["d6c7e11"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["63878ac"] = "-no-custom-assertions -ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["657e964"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["0988c4c"] = "--max-loop-iterations=100"

@llbmc_args_H["199501f"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["1b487ea"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["b273d66"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["cf1f2ac"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["ebee301"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["5275b1e"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["b7ebc61"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"
@llbmc_args_H["5cd6461"] = "-ignore-missing-function-bodies --no-max-loop-iterations-checks"

# OTHER LLBMC ARGS
# 	llbmc_H["0dc77b6"] = "llbmc --max-loop-iterations=100 0dc77b6/variant-err/0dc77b6.bc"
# 	llbmc_H["1f758a4"] = "llbmc --max-loop-iterations=100 1f758a4/variant-err/1f758a4.bc"
# 	llbmc_H["208d898"] = "llbmc -ignore-missing-function-bodies --no-max-loop-iterations-checks 208d898/variant-err/208d898.bc"
# 	llbmc_H["218ad12"] = "llbmc -leak-check --max-loop-iterations=100 218ad12/variant-err/218ad12.bc"

def folder (folder, file, ext)
	"#{folder}/#{@files_H[file]}#{file}/#{file}#{ext}"
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

def variantBC (file)
	folder("variant", file, ".bc")
end

def oracleBC (file)
	folder("oracle", file, ".bc")
end

puts "\n"*20
puts "Run test"

if (ARGV[0] != nil)
	if (@files_H.keys.include?(ARGV[0]))
		file = ARGV[0]
		puts
		puts "--------------------------------------------------------------"
		puts "  TESTING " + file
		puts
		puts "  source size: " + run_command("stat --printf=\"%s\" #{source(file)}") + "B"
		puts "  oracle size: " + run_command("stat --printf=\"%s\" #{oracle(file)}") + "B"
		puts
		run_command("mkdir -p variant/#{@files_H[file]}#{file}")
		puts "  " + command = "clang -E #{@variant_config_H[file]} -o #{variant(file)} #{source(file)}"
		puts "\n" + run_command(command)
		puts
		run_command("mkdir -p target/#{@files_H[file]}#{file}")
		puts "  " + command = "java -jar reconfigurator.jar -source #{source(file)} -target #{target(file)} -oracle #{oracle(file)}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  FRAMA-C"
		puts
		puts "  " + command = "frama-c -val -quiet #{variant(file)}"
		puts "\n" + run_command(command)
		puts
		puts "  " + command = "frama-c -val -quiet #{oracle(file)}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  CLANG"
		puts
		puts "  " + command = "clang -c -g -emit-llvm -Wall -o #{variantBC(file)} #{variant(file)}"
		puts "\n" + run_command(command)
		puts
		puts "  " + command = "clang -c -g -emit-llvm -Wall -o #{oracleBC(file)} #{oracle(file)}"
		puts "\n" + run_command(command)
		puts
		puts "--------------------------------------------------------------"
		puts "  LLBMC"
		puts
		puts "  " + command = "llbmc #{@llbmc_args_H[file]} #{variantBC(file)}"
		puts "\n" + run_command(command)
		puts
		puts "  " + command = "llbmc #{@llbmc_args_H[file]} #{oracleBC(file)}"
		puts "\n" + run_command(command)
	else
		puts "file not found"
	end
else
	id = 0
	puts " ID  | HASH    | file size (B)   | frama-c (ms)    | clang (ms)      | llbmc (ms)      |"
	puts "     |         | source | oracle | var    | oracle | var    | oracle | var    | oracle |"
	puts "----------------------------------------------------------------------------------------"
	for file in @files_H.keys
		id = id + 1
		print id.to_s.rjust(4, ' ') + " |"
		print file.rjust(8, ' ') + " |"
		
		run_command("mkdir -p variant/#{@files_H[file]}#{file}")
		run_command("clang -E #{@variant_config_H[file]} -o #{variant(file)} #{source(file)}")

		run_command("mkdir -p target/#{@files_H[file]}#{file}")
		run_command("java -jar reconfigurator.jar -source #{source(file)} -target #{target(file)} -oracle #{oracle(file)}")

		print run_command("stat --printf=\"%s\" #{source(file)}").rjust(7, ' ') + " |"
		print run_command("stat --printf=\"%s\" #{oracle(file)}").rjust(7, ' ') + " |"

		print command_median("frama-c -val -quiet #{variant(file)}").rjust(7, ' ') + " |"
		print command_median("frama-c -val -quiet #{oracle(file)}").rjust(7, ' ') + " |"
		
		print command_median("clang -c -g -emit-llvm -Wall -o #{variantBC(file)} #{variant(file)}").rjust(7, ' ') + " |"
		print command_median("clang -c -g -emit-llvm -Wall -o #{oracleBC(file)} #{oracle(file)}").rjust(7, ' ') + " |"

		print command_median("llbmc #{@llbmc_args_H[file]} #{variantBC(file)}").rjust(7, ' ') + " |"
		print command_median("llbmc #{@llbmc_args_H[file]} #{oracleBC(file)}").rjust(7, ' ') + " |"

		puts
	end
end

puts "End test"