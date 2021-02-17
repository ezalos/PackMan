#include <unistd.h>
#include "head.h"

int main(int ac, char **av)
{
    write(1, "Hello World\n", 12);

    goto random;

    random:
        ft_cypher((void*)0x1234567890abcdef, 10, 0x42);
}

0000000000001139 <main>:
    1139:       f3 0f 1e fa             endbr64 
    113d:       55                      push   rbp
    113e:       48 89 e5                mov    rbp,rsp
    1141:       48 83 ec 10             sub    rsp,0x10
    1145:       89 7d fc                mov    DWORD PTR [rbp-0x4],edi
    1148:       48 89 75 f0             mov    QWORD PTR [rbp-0x10],rsi
    114c:       ba 0c 00 00 00          mov    edx,0xc
    1151:       48 8d 35 ac 0e 00 00    lea    rsi,[rip+0xeac]        # 2004 <_IO_stdin_used+0x4>
    1158:       bf 01 00 00 00          mov    edi,0x1
    115d:       e8 ce fe ff ff          call   1030 <write@plt>
    1162:       90                      nop
    1163:       ba 42 00 00 00          mov    edx,0x42
    1168:       be 0a 00 00 00          mov    esi,0xa
    116d:       48 bf ef cd ab 90 78    movabs rdi,0x1234567890abcdef
    1174:       56 34 12 
    1177:       e8 14 00 00 00          call   1190 <ft_cypher>
    117c:       b8 00 00 00 00          mov    eax,0x0
    1181:       c9                      leave  
    1182:       c3                      ret    
    1183:       66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
    118a:       00 00 00 
    118d:       0f 1f 00                nop    DWORD PTR [rax]
    