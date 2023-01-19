mod doubling;
mod hat;
mod resizable_arr;

use resizable_arr::ResizableArray;

use crate::doubling::DoublingResizableArray;
use crate::hat::HATResizableArray;

const N: i32 = 2 << 20;

fn main() {
    let mut double: DoublingResizableArray<i32> = DoublingResizableArray::new();
    for i in 0..N {
        double.push_back(i);
    }

    let mut hat: HATResizableArray<i32> = HATResizableArray::new();
    for i in 0..N {
        //hat.push_back(i);
    }
}
