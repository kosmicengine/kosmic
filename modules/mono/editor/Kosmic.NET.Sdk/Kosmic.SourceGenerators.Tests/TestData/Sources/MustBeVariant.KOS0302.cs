using Kosmic;

public class MustBeVariantKS0302
{
    public void MethodOk<[MustBeVariant] T>()
    {
        // T is guaranteed to be a Variant-compatible type because it's annotated with the [MustBeVariant] attribute, so it can be used here.
        new ExampleClass<T>();
        Method<T>();
    }

    public void MethodFail<T>()
    {
        // These two calls raise a KS0302 diagnostic error: T is not valid here because it may not a Variant type and method call and class require it.
        new ExampleClass<{|KS0302:T|}>();
        Method<{|KS0302:T|}>();
    }

    public void Method<[MustBeVariant] T>()
    {
    }
}

public class ExampleClass<[MustBeVariant] T>
{

}
