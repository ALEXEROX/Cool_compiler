package cool.runtime;

public class String extends Object{
    private java.lang.String value;

    public String() { this.value = ""; }
    public String(java.lang.String s) { this.value = s; }

    public java.lang.String getValue() { return value; }

    public String concat(String other) {
        return new String(this.value + other.value);
    }
}
