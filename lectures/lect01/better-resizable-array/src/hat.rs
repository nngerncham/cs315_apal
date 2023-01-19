const DEFAULT_INIT_K: usize = 3;

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
