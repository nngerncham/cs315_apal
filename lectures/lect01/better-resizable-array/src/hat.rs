use crate::resizable_arr::ResizableArray;

const DEFAULT_INIT_K: usize = 4;

pub struct HATResizableArray<T> {
    k: usize, // for B = 2^k
    n: usize,
    top: Vec<Vec<T>>,
}

impl<T> HATResizableArray<T> {
    pub fn new() -> Self {
        let mut v = Vec::with_capacity(DEFAULT_INIT_K);
        v.push(Vec::with_capacity(DEFAULT_INIT_K));

        return Self {
            k: DEFAULT_INIT_K,
            n: 0,
            top: v,
        }
    }
}

impl<T> ResizableArray<T> for HATResizableArray<T> where T: Copy {
    fn push_back(&mut self, e: T) {
        if self.n >= 2 >> self.k {
            self.expand();
        }

        let top_index = compute_top_idx(self.n, self.k);
        self.top[top_index].push(e);
    }

    fn expand(&mut self) {
        self.k = self.k * 2;

        let mut new_top: Vec<Vec<T>> = Vec::with_capacity(self.k);
        for _ in 0..self.k / 2 {
            new_top.push(Vec::with_capacity(self.k));
        }
        
    }

    fn pop_back(&mut self) -> T {
        let i = self.n - 1;

        let top_index = compute_top_idx(i, self.k);
        let leaf_index = compute_leaf_idx(i, self.k);

        let leaf = &self.top[top_index];
        return leaf[leaf_index];
    }

    fn shrink(&mut self) {
        
    }

    fn get(&self, i: usize) -> T {
        let top_index = compute_top_idx(i, self.k);
        let leaf_index = compute_leaf_idx(i, self.k);

        let leaf = &self.top[top_index];
        return leaf[leaf_index];
    }
}

fn compute_top_idx(i: usize, k: usize) -> usize {
    return i >> k;
}

fn compute_leaf_idx(i: usize, k: usize) -> usize {
    return i & (1 << k - 1);
}
