use crate::resizable_arr::ResizableArray;

const DEFAULT_INIT_CAP: usize = 16;

pub struct DoublingResizableArray<T> {
    cap: usize,
    n: usize,
    arr: Vec<T>,
}

impl<T> DoublingResizableArray<T>
where
    T: Clone,
{
    pub fn new() -> Self {
        return Self {
            cap: DEFAULT_INIT_CAP,
            n: 0,
            arr: Vec::with_capacity(DEFAULT_INIT_CAP),
        };
    }

    fn is_full(&self) -> bool {
        return self.n == self.cap;
    }

    fn expand(&mut self) {
        self.cap = self.cap * 2;

        let mut new_arr = Vec::<T>::with_capacity(self.cap);
        for e in &self.arr {
            new_arr.push(e.clone());
        }
    }

    fn shrink(&mut self) {
        self.cap = self.cap / 2;
        self.arr.truncate(self.cap);
    }
}

impl<T> ResizableArray<T> for DoublingResizableArray<T>
where
    T: Copy,
{
    fn push_back(&mut self, e: T) {
        if self.is_full() {
            self.expand();
        }
        self.arr.push(e);
        self.n = self.n + 1;
    }

    fn pop_back(&mut self) -> T {
        if self.n < self.cap / 4 {
            self.shrink();
        }

        self.n = self.n - 1;
        return self.arr.pop().unwrap();
    }

    fn get(&self, i: usize) -> T {
        return self.arr[i];
    }
}
