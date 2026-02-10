.alias myconst c32 as (0.0, 1.0, 0.0000305185094759, 0.00001525902189)
.alias myconst2 c33 as (0.0009765625, 0.03125, 32.0, 0.03225806451)

.alias outpos o0 as position
.alias outcol o1 as color
.alias outtex0 o2.xyzw as texcoord0

.alias inpos      v0
.alias inuvc      v1

.alias projection c0-c3

main:
    mov r0.xyz,  inpos.xyz
    mov r0.w,   myconst.y

proj:
    dp4 outpos.x, projection[0], r0
    dp4 outpos.y, projection[1], r0
    dp4 outpos.z, projection[2], r0
    dp4 outpos.w, projection[3], r0

tex:
    mul outtex0.xy, myconst.zz, inuvc.xy
    mov outtex0.zw, myconst.xx

color:
    mov r0.xyz, inuvc.zzz
	mul r0.xy, myconst2.xy, r0.xy
	flr r0.xy, r0.xy

    mul r0, myconst2.yyyy, r0
	flr r1, r0
	add r0, r0, -r1
	mul r0, myconst2.zzzz, r0

	mul outcol.xyz, myconst2.www, r0.xyz
    mov outcol.w, myconst.y
    nop
    end
    