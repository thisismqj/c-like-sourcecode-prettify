int fib(int n) {
    if (n <= 2)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

int main() {
    int n, x;
    x=getNum();
    n=(x*x*x*x*x+2*x*x*x+x)/(x*x*x*x+2*x*x+1);
    printf("fib: %d",fib(n));
    return 0;
}
