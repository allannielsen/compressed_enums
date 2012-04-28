#!/usr/bin/env ruby

require "open4"
require 'optparse'
require "fileutils"

options = {}

OptionParser.new do |opts|
    opts.banner = "Usage: version.rb [options]"

    opts.on("-s", "--src-dir <path>", "Project root dir") do |v|
        options[:source] = File.expand_path(v)
    end

    opts.on("-o", "--output <path>", "Output dir") do |v|
        options[:output] = File.expand_path(v)
    end

    opts.on("-c", "--compiler [TYPE]", [:gcc, :gmbhost, :clang, :intel], "Select compiler: gmbhost|gcc|clang|intel") do |v|
        options[:compiler] = v
    end

    opts.on("--cc [path]", "Path to c compiler") do |v|
        options[:cc] = File.expand_path(v)
    end

    opts.on("--cxx [path]", "Path to cxx compiler") do |v|
        options[:cxx] = File.expand_path(v)
    end

    opts.on("-b", "--build-mode [MODE]", [:release, :debug], "Select mode: release|debug") do |v|
        options[:build_mode] = v
    end

    opts.on("-t", "--type [TYPE]", [:target, :host], "Select type: target|host") do |v|
        options[:type] = v
    end

    opts.on("-e", "--eclipse", "Use eclipse generator") do
	options[:eclipse] = true
    end

    opts.on_tail("-h", "--help", "Print help") do |v|
        puts opts
        exit
    end
end.parse!

if options[:source].nil?
    options[:source] = File.expand_path(".")
end

if options[:compiler].nil?
    options[:compiler] = :gmbhost
end

if options[:build_mode].nil?
    options[:build_mode] = :release
end

if options[:type].nil?
    options[:type] = :host
end

def run cmd
    puts "Running: #{cmd}"
    return Open4::popen4(cmd) do |pid, stdin, stdout, stderr|
        $stdout.write(stdout.read)
        $stderr.write(stderr.read)
    end
end

cmake_args = []
default_output = ".build"

case options[:build_mode]
when :release; 
    default_output += "-release"
    cmake_args << "-DCMAKE_BUILD_TYPE=Release"

when :debug;
    cmake_args << "-DCMAKE_BUILD_TYPE=Debug"
    default_output += "-debug"

end

case options[:compiler]
when :gcc;
    cmake_args << "-DCMAKE_USER_MAKE_RULES_OVERRIDE=#{options[:source]}/.cmake/GccOverrides.txt"
    default_output += "-gcc"

when :gmbhost
    cmake_args << "-DCMAKE_USER_MAKE_RULES_OVERRIDE=#{options[:source]}/.cmake/GccOverrides.txt"
    default_output += "-gmbhost"

when :clang
    cmake_args << "-DCMAKE_USER_MAKE_RULES_OVERRIDE=#{options[:source]}/.cmake/ClangOverrides.txt"
    default_output += "-clang"

when :intel
    cmake_args << "-DCMAKE_USER_MAKE_RULES_OVERRIDE=#{options[:source]}/.cmake/IntelOverrides.txt"
    default_output += "-icc"

end

case options[:type]
when :host;
    cmake_args << "-DGMB_TARGET=OFF"
    default_output += "-host"

when :target;
    cmake_args << "-DGMB_TARGET=ON"
    default_output += "-target"

end

if not options[:eclipse].nil?
   cmake_args << "-G \"Eclipse CDT4 - Unix Makefiles\""
end


if options[:cc].nil?
    case options[:compiler]
    when :gcc;     ENV["CC"] = "gcc"
    when :gmbhost; ENV["CC"] = "/usr/local/x86_64-gmbhost-linux-gnu/bin/gcc"
    when :clang;   ENV["CC"] = "clang"
    when :intel;   ENV["CC"] = "icc"
    end
else
    ENV["CC"] = options[:cc]
end

if options[:cxx].nil?
    case options[:compiler]
    when :gcc;     ENV["CXX"] = "g++"
    when :gmbhost; ENV["CXX"] = "/usr/local/x86_64-gmbhost-linux-gnu/bin/g++"
    when :clang;   ENV["CXX"] = "clang++"
    when :intel;   ENV["CXX"] = "icpc"
    end
else
    ENV["CXX"] = options[:cxx]
end

if options[:output].nil?
    options[:output] = default_output
end

if( Dir.exist?(options[:output]) )
    puts "Output directory: #{options[:output]} allready exists"
    exit -1
end

Dir.mkdir(options[:output])
Dir.chdir(options[:output])

exit run("cmake #{cmake_args.join(" ")} #{options[:source]}").to_i


