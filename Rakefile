cinc   = '-Isrc/inc'
cflag  = "-Wall -finline-functions -nostdinc -fno-builtin"
pgrep  = "grep --color -e 'error' -e 'error' -e '^'"

sh "mkdir bin" if not File.exists? 'bin'

task :default => :bochs

task :bochs => :build do
  sh "bochs -q -f .bochsrc"
end

task :nm => :build do 
  sh 'cat main.nmtab'
end

task :debug => :build do
  sh "bochs-dbg -q -f .bochsrc"
end
  
task :build => 'bin/kernel.img'

task :clean do
  sh "rm -rf bin/* src/kern/hwint.S .bochsout"
end

## helpers ##
task :todo do 
  sh "grep -r -n 'TODO' ./src --color"
end

#######################################################################
# => kernel.img
#######################################################################
file 'bin/kernel.img' => ['bin/boot.bin', 'bin/main.bin'] do
  sh "cat bin/boot.bin bin/main.bin > bin/kernel.img"
end

#######################################################################
# => boot.bin
#######################################################################
file 'bin/boot.o' => ['src/boot/boot.S'] do
  sh "nasm -f elf -o bin/boot.o src/boot/boot.S"
end

file 'bin/boot.bin' => ['bin/boot.o', 'boot.ld'] do 
  sh "ld bin/boot.o -o bin/boot.bin -e c -T boot.ld"
end

#######################################################################
# mainly C part
# => main.bin
#######################################################################

hfiles = [
  'src/inc/buf.h',
  'src/inc/kern.h',
  'src/inc/param.h',
  'src/inc/proc.h',
  'src/inc/unistd.h',
  #
  'src/inc/conf.h',
  'src/inc/hd.h',
  # 
  'src/inc/idt.h',
  'src/inc/gdt.h',
  'src/inc/mmu.h',
  'src/inc/tss.h',
  'src/inc/asm.h',
  'src/inc/x86.h'
]

cfiles = [
  'src/kern/tty.c',
  'src/kern/syscall.c',
  'src/kern/sched.c',
  'src/kern/seg.c',
  'src/kern/trap.c',
  'src/kern/page.c',
  'src/kern/timer.c',
  #
  'src/drv/buf.c',
  'src/drv/conf.c',
  'src/drv/hd.c',
  #
  'src/lib/str.c',
  #
  'src/kern/main.c'
]

sfiles = [
  'src/kern/hwint.S',
  'src/kern/entry.S'
]

ofiles = (cfiles + sfiles).map{|fn| 'bin/'+File.basename(fn).ext('o') }

cfiles.each do |fn_c|
  fn_o = 'bin/'+File.basename(fn_c).ext('o')
  file fn_o => [fn_c, *hfiles] do
    sh "gcc #{cflag} #{cinc} -o #{fn_o} -c #{fn_c} 2>&1"
  end
end

sfiles.each do |fn_s|
  fn_o = 'bin/'+File.basename(fn_s).ext('o')
  file fn_o => [fn_s, *hfiles] do
    sh "nasm -f elf -o #{fn_o} #{fn_s}"
  end
end

######################################################################################3

file 'bin/main.bin' => 'bin/main.elf' do
  sh "objcopy -R .pdr -R .comment -R .note -S -O binary bin/main.elf bin/main.bin"
end

file 'bin/main.elf' => ofiles + ['main.ld'] do
  sh "ld #{ofiles * ' '} -o bin/main.elf -e c -T main.ld"
  sh "(nm bin/main.elf | sort) > main.sym"
end

# hwint.S is generated by a ruby script
file 'src/kern/hwint.S' => 'src/kern/hwint.S.rb' do 
  sh 'ruby src/kern/hwint.S.rb > src/kern/hwint.S'
end


