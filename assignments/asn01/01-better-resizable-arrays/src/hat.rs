use crate::resizable_arr::ResizableArray;

const DEFAULT_INIT_K: usize = 2;

pub struct HATResizableArray<T> {
    k: usize, // for B = 2^k
    n: usize,
    top: Vec<Vec<T>>,
}

impl<T> HATResizableArray<T>
where
    T: Copy,
{
    pub fn new() -> Self {
        let mut v = Vec::with_capacity(2 << DEFAULT_INIT_K);
        v.push(Vec::with_capacity(2 << DEFAULT_INIT_K));

        return Self {
            k: DEFAULT_INIT_K,
            n: 0,
            top: v,
        };
    }

    fn is_full(&self) -> bool {
        let b = 1 << self.k;
        return self.n == b * b;
    }

    pub fn get_top(&self) -> &Vec<Vec<T>> {
        return &self.top;
    }

    fn expand(&mut self) {
        let new_b = 2 << self.k;

        let mut new_arr: Vec<Vec<T>> = Vec::with_capacity(new_b);
        for i in 0..1 << (self.k - 1) {
            let v1 = &self.top[i * 2];
            let v2 = &self.top[i * 2 + 1];

            let mut new_leaf: Vec<T> = Vec::with_capacity(new_b);
            new_leaf.extend(v1);
            new_leaf.extend(v2);

            new_arr.push(new_leaf);
        }

        self.top = new_arr;
        self.k = self.k + 1;
    }

    fn shrink(&mut self) {
        // TODO
    }
}

impl<T> ResizableArray<T> for HATResizableArray<T>
where
    T: Copy,
{
    fn push_back(&mut self, e: T) {
        if self.is_full() {
            self.expand();
        }

        let top_index = compute_top_idx(self.n, self.k);
        if self.top.len() == top_index {
            self.top.push(Vec::with_capacity(1 << self.k));
        }

        self.top[top_index].push(e);
        self.n = self.n + 1;
    }

    fn pop_back(&mut self) -> T {
        let i = self.n - 1;

        let top_index = compute_top_idx(i, self.k);
        let leaf_index = compute_leaf_idx(i, self.k);

        let leaf = &self.top[top_index];
        self.n = self.n - 1;
        return leaf[leaf_index];
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
