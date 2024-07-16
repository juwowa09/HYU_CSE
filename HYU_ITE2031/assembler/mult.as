        lw      0       2       mcand
        lw      0       3       mplier
        lw      0       4       mask
        lw      0       5       max
loop    lw      0       6       neg1    using temp register 6
        add     5       6       5       add (max, -1)
        beq     0       5       done    max bit (15) clear, go to done
        nor     3       4       6       ~(mplier | mask)
        beq     0       6       result  mplier's last bit is 1, go to result
        beq     0       0       next    mplier's last bit is 0, go to next
result  add     1       2       1       add (register 1, mcand)
next    add     2       2       2       mcand left shift
        add     4       4       4       mask left shift
        lw      0       6       one     using temp register 6
        add     4       6       4       add (mask, 1)
        beq     0       0       loop    loop
done    halt                            end of program
mcand   .fill   32766
mplier  .fill   12328
mask    .fill   -2                      1111 1111 1111 1110 (because of nor)
max     .fill   16
neg1    .fill   -1
one     .fill   1
