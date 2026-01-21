package cool.runtime;

public class Object {
    public Object abort() {
        System.exit(1);
        return this;
    }
}
