pub trait ResizableArray<T>
where
    T: Copy,
{
    fn push_back(&mut self, e: T);
    fn pop_back(&mut self) -> T;
    fn get(&self, i: usize) -> T;
}
