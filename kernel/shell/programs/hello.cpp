int hello_loop = 0;
void hello() {
    print_string("Hello, world!\n\r");
    hello_loop++;
    if (hello_loop >= 10) {
        proc_quit();
    }
}