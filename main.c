void hcf(void)
{
    for (;;)
    {
        asm("wfi");
    }
}

void main(void)
{
    hcf();
}