first compile code using gcc 'gcc -g array_crackme.c -o array_crackme'.
run it using 'gdb ./array_crackme'.

                                      GDB
in GDB:
make a breakpoint 'b main'.
run it 'r'.

lets disassble it 'disas main'.
you probely will see something like this:
   0x0000000000400466 <+0>:	push   %rbp
   0x0000000000400467 <+1>:	mov    %rsp,%rbp
   0x000000000040046a <+4>:	sub    $0x20,%rsp
=> 0x000000000040046e <+8>:	movl   $0x0,-0x8(%rbp)           //  'sum' in C.
   0x0000000000400475 <+15>:	movl   $0x0,-0x4(%rbp)         //  'i' in C.
   0x000000000040047c <+22>:	jmp    0x400499 <main+51>      //  jump to ->  <main+51>.
   0x000000000040047e <+24>:	mov    -0x4(%rbp),%edx         // move 'i' to %edx.
   0x0000000000400481 <+27>:	mov    %edx,%eax               // copy %edx to %eax.
   0x0000000000400483 <+29>:	shl    $0x2,%eax               // shift left %eax^2 (i * (2^2)).
   0x0000000000400486 <+32>:	add    %edx,%eax               // add i to i * (2^2) // i + i * 4.
   0x0000000000400488 <+34>:	add    %eax,%eax               // add i * 5 + i * 5.
   0x000000000040048a <+36>:	mov    %eax,%edx               // copy %eax to %edx i = i * 5.
   0x000000000040048c <+38>:	mov    -0x4(%rbp),%eax         // move 'i' to %eax return i to original form.
   0x000000000040048f <+41>:	cltq                           // change 32-bit to 64-bit.
   0x0000000000400491 <+43>:	mov    %edx,-0x20(%rbp,%rax,4) // array in asm
   0x0000000000400495 <+47>:	addl   $0x1,-0x4(%rbp)         // add 1 to 'i'. // i++ in C.
   0x0000000000400499 <+51>:	cmpl   $0x4,-0x4(%rbp)         // compare 4 to 'i'. // i < 5 in C.
   0x000000000040049d <+55>:	jle    0x40047e <main+24>      // jump to -> <main+24> if 'i' is less or equale to 4.
   0x000000000040049f <+57>:	movl   $0x0,-0x4(%rbp)         // set 'i' to 0.
   0x00000000004004a6 <+64>:	jmp    0x4004b8 <main+82>      // jump to -> main+82.
   0x00000000004004a8 <+66>:	mov    -0x4(%rbp),%eax         // move 'i' to %eax
   0x00000000004004ab <+69>:	cltq                           // set 32-bit to 64-bit.
   0x00000000004004ad <+71>:	mov    -0x20(%rbp,%rax,4),%eax // mov array to %eax.
   0x00000000004004b1 <+75>:	add    %eax,-0x8(%rbp)         // add %eax to sum // sum += num[i] in C. 
   0x00000000004004b4 <+78>:	addl   $0x1,-0x4(%rbp)         // add 1 to 'i' i++ in C.
   0x00000000004004b8 <+82>:	cmpl   $0x4,-0x4(%rbp)         // compare 4 to 'i' // 'i < 5' in C. 
   0x00000000004004bc <+86>:	jle    0x4004a8 <main+66>      // jump to -> <main+66> if less or equale to 4.
   0x00000000004004be <+88>:	mov    -0x8(%rbp),%eax         // move sum to %eax. (finale argument for printf).
   0x00000000004004c1 <+91>:	mov    %eax,%esi               // mov %eax to %esi (1st argument for printf).
   0x00000000004004c3 <+93>:	mov    $0x401180,%edi          // mov address to %edi (2nd argument).
   0x00000000004004c8 <+98>:	mov    $0x0,%eax               // clear %eax using 0x0.
   0x00000000004004cd <+103>:	call   0x400370 <printf@plt>   // call value '0x400370' to print using printf.
   0x00000000004004d2 <+108>:	mov    $0x0,%eax               // clear printf value.
   0x00000000004004d7 <+113>:	leave
   0x00000000004004d8 <+114>:	ret
   
  nothing to fear its easy if you break it down let's get started.

                                              ASM description.
  first we have:
    0x000000000040046e <+8>:	movl   $0x0,-0x8(%rbp)  // it's sum u can get that info by looking in here: 0x0000000000400495 <+47>:	addl   $0x1,-0x4(%rbp) it means add 1 to 'i' i++ in C.
    0x0000000000400475 <+15>:	movl   $0x0,-0x4(%rbp)  // this is 'i'.
    
  Now lets cover 1st for loop:
    0x000000000040047c <+22>:	jmp    0x400499 <main+51> // we will go to <main+51>.
    0x0000000000400499 <+51>:	cmpl   $0x4,-0x4(%rbp)  // compare 4 to -0x4 our 'i'. Why 4, we use 4 because its an integer and we can write it like i <= 4 because cycle have to work while i < 5.
  use 'ni' to go down.
  You will see 0x000000000040049d <+55>:	jle    0x40047e <main+24> // jle stand for jump if less or equale. So you jump to <main+24>.
  now we are in for loop. 
  
  for(...){
      // we are here!
  }

  what can we see:
  
   0x000000000040047e <+24>:	mov    -0x4(%rbp),%edx
   0x0000000000400481 <+27>:	mov    %edx,%eax
   0x0000000000400483 <+29>:	shl    $0x2,%eax
   0x0000000000400486 <+32>:	add    %edx,%eax
   0x0000000000400488 <+34>:	add    %eax,%eax
   0x000000000040048a <+36>:	mov    %eax,%edx
   0x000000000040048c <+38>:	mov    -0x4(%rbp),%eax
   0x000000000040048f <+41>:	cltq
   0x0000000000400491 <+43>:	mov    %edx,-0x20(%rbp,%rax,4)
   0x0000000000400495 <+47>:	addl   $0x1,-0x4(%rbp)

  1st copy 'i' to %edx.
  2nd copy %edx to %eax.
  3rd we could see an imul(multiplication) but my compiler wanted to make it more faster so i will use shift left and adding.
  You could see righ now a not familiar instuction 'shl' it's shift left instruction.
  how does it work: Our compiler doesn't want to multiply beacase the number is not that complicated so it will use shift left so for examle 'i' is 3.
    1. %edx = 3                  // mov -0x4(%rbp),%edx
    2. %edx = 3, %eax =3.        // %edx,%eax
    3. %eax = 12(3 * (2^2) = 12) // shl $0x2, %eax
    4. %eax = 12 + 3 = 15        // add %edx, %eax
    5. add = 15 + 15 = 30        // add %eax, %eax
    6. eax = 3                   // mov -0x4(%rbp),%eax // return back 'i' to original state
  now u see how its done not hard.

  type 'ni'.
  You can see a new 'cltq' instruction it capable of making from 32-bit -> 64-bit we need that for array.
  then we move it from 32-bit to 64-bit, we get to an array it's self.
  
  array in assembly:
    space for array is -0x20 u can calculate it by: 5*4. we take array space int[5] <- this 5 and multiply by bytes of a variable: int is 4 bytes. so we have 5*4.

   complex addresation:
       (base, index, scale).
       mov %edx, -0x20(rbp, rax, 4).
           more detales:
               1. displacement -> -0x20 we displace from %rbp to start of array.
               2. base -> it's a base part array pointer. It point's to start of all function's local memory.
               3. index it's our index 'i' = %rax.
               4. scale it's the size of one element in our array in bytes. because we have 'int' we use 4 bytes.

   calculation process:
       address = displacement + base + (index * scale);
       address = -0x20 + %rbp + (%rax * 4);
  we move our array in to %eax
  then we add %eax to sum. sum += num[i] in C.
  and add 1 to -0x4('i') i++ in C.

  then we clear i:    0x000000000040049f <+57>:	movl   $0x0,-0x4(%rbp)

  same with second for loop.
  move to <main+82>: 0x00000000004004a6 <+64>:	jmp    0x4004b8 <main+82>, compare i to 4: 0x00000000004004b8 <+82>:	cmpl   $0x4,-0x4(%rbp) then jump if less or equale to <main+66>: 0x00000000004004bc <+86>:	jle    0x4004a8 <main+66>.
  move  'i' to %eax.
  use cltq.
  move array: -0x20(%rbp,%rax,4) to %eax.
  add %eax, sum. sum += num[i] in C.
  and add 1 to 0x4('i'): 0x00000000004004b4 <+78>:	addl   $0x1,-0x4(%rbp) // i++ in C.

  last part:
   0x00000000004004be <+88>:	mov    -0x8(%rbp),%eax // move sum to %eax. (finale argument for printf).
   0x00000000004004c1 <+91>:	mov    %eax,%esi       // mov %eax to %esi (1st argument for printf).
   0x00000000004004c3 <+93>:	mov    $0x401180,%edi  // mov address to %edi (2nd argument).
   0x00000000004004c8 <+98>:	mov    $0x0,%eax       // clear %eax using 0x0.
   0x00000000004004cd <+103>:	call   0x400370 <printf@plt>   // printf in C
   
                                                       HOW TO CRACK: 
  You can crack 2 variables, 2 for loops, 2 array, and skip loops.
    1st. variables 
          if you want to crack variables you can check there value by 'x/5b 0x40046e' you will see something like that: 0x40046e <main+8>:	0xc7	0x45	0xf8	0x00	0x00.
          we need the 4th value.
          write 'set {char}0x400471 = 0x02' we write differnt memory address because add + more bytes (0xc7	0x45	0xf8).
          and check value again 'x/5b 0x40046e'.
          same with second variable.
          if its a long integer like 99 better to use {int}
    2nd. for loop
          if u want to crack a for loop u have to use the same way but we need to get the comperesent address but add a breakpoint.
          'b *0x400499' set a breakpoint.
          'x/5b 0x400499' take the 4th value.
          'set {char}0x40049c 0x01' and check the value. // we use char because 0x01 is only 1 byte its not 99 or smth like that.
          after that type 'c' to contioue.
          we use char because its more safe and we need only 1 byte insted of 4 which integer will give us. That can lead to an error.
    3rd. how to crack an array
          if you want to crck array you can add a number to it clasic output: 100 = 0+10+20+30+40, cracked: 199 = 99+10+20+30+40.
          'set {int}($rbp-0x20) = 99' set the first to 99 
          'x/5xw $rbp-0x20' the first one: 0x63 is 99 in hexadecimal.
          and press 'c' to continue.
    4th. how to bypass a for loop.
          use 'set $rip = address'
