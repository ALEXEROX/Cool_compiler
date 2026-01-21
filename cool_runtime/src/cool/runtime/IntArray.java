package cool.runtime;

import java.util.ArrayList;

public class IntArray {
    private ArrayList<Integer> elems = new ArrayList<>();

    public int length() {
        return elems.size();
    }

    public int get(int i) {
        return elems.get(i);
    }

    public int set(int i, int val) {
        elems.set(i, val);
        return val;
    }

    public int append(int val) {
        elems.add(val);
        return val;
    }
}
