package java.lang;

public class Math {
	public static final double PI = 3.14159265358979;

	public static int abs(int a) {
		return a >= 0 ? a : -a;
	}
	public static native double sqrt(double a);
	public static native double sin(double a);
	public static native double cos(double a);
	public static native double tan(double a);
	public static native double atan(double a);
	public static native double atan2(double y, double x);
	public static native double pow(double a, double b);
}
