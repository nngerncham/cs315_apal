mod alpha;
mod doubling;
mod hat;
mod resizable_arr;

use csv::Writer;

use crate::resizable_arr::ResizableArray;

use crate::alpha::AlphaResizableArray;
use crate::doubling::DoublingResizableArray;
use crate::hat::HATResizableArray;

use core::arch::x86_64::_rdtsc;
use project_root::get_project_root;

const N: usize = 1 << 24;

fn time_push<A: ResizableArray<usize>>(lst: &mut A) -> Vec<u64> {
    let mut time_diffs = Vec::with_capacity(N);

    for i in 0..=N {
        unsafe {
            let start = _rdtsc();
            lst.push_back(i);
            let stop = _rdtsc();
            time_diffs.push(stop - start);
        }
    }

    time_diffs
}

fn time_get<A: ResizableArray<usize>>(lst: &mut A) -> Vec<u64> {
    let mut time_diffs = Vec::with_capacity(N);

    for i in 0..=N {
        unsafe {
            let start = _rdtsc();
            lst.get(i);
            let stop = _rdtsc();
            time_diffs.push(stop - start);
        }
    }

    time_diffs
}

fn main() {
    let mut double: DoublingResizableArray<usize> = DoublingResizableArray::new();
    let dpush_latency = time_push(&mut double);
    let dget_latency = time_get(&mut double);

    let mut hat: HATResizableArray<usize> = HATResizableArray::new();
    let hpush_latency = time_push(&mut hat);
    let hget_latency = time_get(&mut hat);

    let mut alpha: AlphaResizableArray<usize> = AlphaResizableArray::new();
    let apush_latency = time_push(&mut alpha);
    let aget_latency = time_get(&mut alpha);

    let path = get_project_root().unwrap().join("results.csv");
    let mut wtr = Writer::from_path(path).unwrap();
    let _res = wtr.write_record(&[
        "i",
        "double_push",
        "double_get",
        "hat_push",
        "hat_get",
        "alpha_push",
        "alpha_get",
    ]);
    for i in 0..=N {
        let _res = wtr.write_record(&[
            i.to_string(),
            dpush_latency[i].to_string(),
            dget_latency[i].to_string(),
            hpush_latency[i].to_string(),
            hget_latency[i].to_string(),
            apush_latency[i].to_string(),
            aget_latency[i].to_string(),
        ]);
    }
}
