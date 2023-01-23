pub trait ResizableArray<T>
where
    T: Copy,
{
    fn push_back(&mut self, e: T);
    fn expand(&mut self);

    fn pop_back(&mut self) -> T;
    fn shrink(&mut self);

    fn get(&self, i: usize) -> T;
}
