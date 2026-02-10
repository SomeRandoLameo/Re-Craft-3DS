.alias myconst c32 as (0.0, 1.0, 0.00392156862745, 0.0)

.alias outpos o0 as position
.alias outcol o1 as color
.alias outtex0 o2.xyzw as texcoord0

.alias inpos      v0
.alias intex      v1
.alias incol      v2

.alias projection c0-c3

main:
    mov r0.xy,  inpos.xy
    mov r0.z,   myconst.x
    mov r0.w,   myconst.y

proj:
    dp4 outpos.x, projection[0], r0
    dp4 outpos.y, projection[1], r0
    dp4 outpos.z, projection[2], r0
    dp4 outpos.w, projection[3], r0

tex:
    mov outtex0.xy, intex.xy
    mov outtex0.zw, myconst.xx

color:
    mul r1.xyzw, myconst.zzzz, incol.xyzw
    mov outcol, r1
    nop
    end
    