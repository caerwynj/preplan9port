void init(int argc, char *argv[]) {
	{extern void input_init(int, char *[]); input_init(argc, argv);}
	{extern void main_init(int, char *[]); main_init(argc, argv);}
	{extern void prof_init(int, char *[]); prof_init(argc, argv);}
	{extern void trace_init(int, char *[]); trace_init(argc, argv);}
	{extern void type_init(int, char *[]); type_init(argc, argv);}
	{extern void x86linux_init(int, char *[]); x86linux_init(argc, argv);}
	{extern void zstab_init(int, char *[]); zstab_init(argc, argv);}
}
extern int main(int, char *[]);
