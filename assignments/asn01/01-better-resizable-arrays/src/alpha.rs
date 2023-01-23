use crate::resizable_arr::ResizableArray;

const DEFAULT_INIT_CAP: usize = 16;

pub struct AlphaResizableArray<T> {
    arr: Vec<T>,
}

impl<T> AlphaResizableArray<T>
where
    T: Clone,
{
    pub fn new() -> Self {
        return Self {
            arr: Vec::with_capacity(DEFAULT_INIT_CAP),
        };
    }
}

impl<T> ResizableArray<T> for AlphaResizableArray<T>
where
    T: Copy,
{
    fn push_back(&mut self, e: T) {
        self.arr.push(e);
    }

    fn pop_back(&mut self) -> T {
        return self.arr.pop().unwrap();
    }

    fn get(&self, i: usize) -> T {
        return self.arr[i];
    }
}
