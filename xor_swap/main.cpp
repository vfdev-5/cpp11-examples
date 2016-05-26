//#include <iostream>
//using namespace std;
//void xor_swap(char * a, char * b)
//{
//    if (a == b) return;
//    *a ^= *b;
//    *b ^= *a;
//    *a ^= *b;
//}


//void add_swap(char * a, char * b)
//{
//    if (a == b) return;

//    *a = *a + *b;
//    *b = *a - *b;
//    *a = *a - *b;
//}

//void simple_swap(char * a, char * b)
//{
//    char t = *a;
//    *a = *b;
//    *b = t;
//}

/*

Empty Main()
    0000000000400746 <main>:
      400746:	55                   	push   %rbp
      400747:	48 89 e5             	mov    %rsp,%rbp
      40074a:	b8 00 00 00 00       	mov    $0x0,%eax
      40074f:	5d                   	pop    %rbp
      400750:	c3                   	retq
      400751:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
      400758:	00 00 00
      40075b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Define to char variables
    0000000000400746 <main>:
      400746:	55                   	push   %rbp
      400747:	48 89 e5             	mov    %rsp,%rbp
>      40074a:	c6 45 ff 61          	movb   $0x61,-0x1(%rbp)
>      40074e:	c6 45 fe 62          	movb   $0x62,-0x2(%rbp)
      400752:	b8 00 00 00 00       	mov    $0x0,%eax
      400757:	5d                   	pop    %rbp
      400758:	c3                   	retq
      400759:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)


Define third char as sum of two previous
    0000000000400746 <main>:
      400746:	55                   	push   %rbp
      400747:	48 89 e5             	mov    %rsp,%rbp
>      40074a:	c6 45 ff 61          	movb   $0x61,-0x1(%rbp)
>      40074e:	c6 45 fe 62          	movb   $0x62,-0x2(%rbp)
>      400752:	0f b6 55 ff          	movzbl -0x1(%rbp),%edx
>      400756:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
>      40075a:	01 d0                	add    %edx,%eax
>      40075c:	88 45 fd             	mov    %al,-0x3(%rbp)
      40075f:	b8 00 00 00 00       	mov    $0x0,%eax
      400764:	5d                   	pop    %rbp
      400765:	c3                   	retq
      400766:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
      40076d:	00 00 00



*/
int main()
{
//    cout << "XOR, Add swaps" << endl;
    char a('a'), b('b');
//    char c = a + b;
//    cout << a << ", " << b << endl;
//    add_swap(&a, &b);
//    cout << a << ", " << b << endl;
//    xor_swap(&a, &b);
//    cout << a << ", " << b << endl;
//    simple_swap(&a, &b);
//    cout << a << ", " << b << endl;
    return 0;
}

