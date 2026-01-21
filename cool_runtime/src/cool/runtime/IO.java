package cool.runtime;

import java.util.Scanner;

public class IO extends Object{
    private static final Scanner scanner = new Scanner(System.in);

    public IO out_string(String s) {
        System.out.print(s);
        return this;
    }

    public IO out_int(int i) {
        System.out.print(i);
        return this;
    }

    public String in_string() {
        return scanner.nextLine();
    }

    public int in_int() {
        return scanner.nextInt();
    }
}
