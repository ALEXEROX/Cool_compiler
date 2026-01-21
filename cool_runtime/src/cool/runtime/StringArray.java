package cool.runtime;

import java.util.ArrayList;

public class StringArray {
    private ArrayList<String> elems = new ArrayList<>();

    public int length() {
        return elems.size();
    }

    public String get(int i) {
        return elems.get(i);
    }

    public String set(int i, String val) {
        elems.set(i, val);
        return val;
    }

    public String append(String val) {
        elems.add(val);
        return val;
    }
}
