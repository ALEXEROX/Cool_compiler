package cool.runtime;

import java.util.ArrayList;

public class Array extends Object{
    private ArrayList<Object> elems = new ArrayList<>();

    public int length() {
        return elems.size();
    }

    public Object get(int i) {
        if(i < 0 || i >= elems.size()) {
            abort();
        }
        return elems.get(i);
    }

    public void set(int i, Object val) {
        if(i < 0 || i >= elems.size()) {
            abort();
        }
        elems.set(i, val);
    }

    public void append(Object val) {
        elems.add(val);
    }
}
