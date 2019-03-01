#define PORT 0xbfe48000

void __attribute__((section(".entry_function"))) _start(void)
{
	int i;
	char s[] = "Hello OS!";

	for(i = 0; i < 9; i++)
		*(char*)PORT = *(s+i);

	return;
}