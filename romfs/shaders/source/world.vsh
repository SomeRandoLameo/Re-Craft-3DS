.alias myconst c95 as (0.0, 1.0, 0.0000305185094759, 0.00392156862745)
.alias myconst2 c94 as (0.0, 1.0, 0.5, 1.0)

.alias outpos o0 as position
.alias outcol o1 as color
.alias outtex0 o2.xyzw as texcoord0

.alias inpos      v0
.alias intex      v1
.alias incol      v2
.alias infxyz     v3

.alias projection c0-c3

main:
    mul r0.xyz, myconst.www,  infxyz.xyz
    add r0.xyz, inpos.xyz, r0.xyz
    mov r0.w,   myconst.y

proj:
    dp4 outpos.x, projection[0], r0
    dp4 outpos.y, projection[1], r0
    dp4 outpos.z, projection[2], r0
    dp4 outpos.w, projection[3], r0

tex:
    mul outtex0.xyzw, myconst.zzzz, intex.xyyy

color:
    mul outcol.xyz, myconst.www, incol.xyz
    mov outcol.w, myconst.y
    nop
    end
    