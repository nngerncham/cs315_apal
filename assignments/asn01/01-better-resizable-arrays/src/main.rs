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
use rand::{thread_rng, Rng, random};

const N: usize = 1 << 20;
const M: usize = 100;


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

fn time_scan<A: ResizableArray<usize>>(lst: &mut A) -> Vec<u64> {
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

fn time_rd_access<A: ResizableArray<usize>>(lst: &A) -> Vec<u64> {
    let mut time_diffs = Vec::with_capacity(N);
    let mut rng = thread_rng();

    for _ in 0..=N {
        unsafe {
            let i = rng.gen_range(0..=N);
            let start = _rdtsc();
            lst.get(i);
            let stop = _rdtsc();
            time_diffs.push(stop - start);
        }
    }

    time_diffs
}

fn time_overall() -> (Vec<u64>, Vec<u64>, Vec<u64>) {
    let mut alpha_diffs = Vec::with_capacity(N);
    for _ in 0..M {
        unsafe {
            let start = _rdtsc();
            let mut lst = Vec::new();
            for _ in 0..=N {
                lst.push(random::<usize>());
            }
            let stop = _rdtsc();
            alpha_diffs.push(stop - start);
        }
    }

    let mut double_diffs = Vec::with_capacity(N);
    for _ in 0..M {
        unsafe {
            let start = _rdtsc();
            let mut lst = DoublingResizableArray::new();
            for _ in 0..=N {
                lst.push_back(random::<usize>());
            }
            let stop = _rdtsc();
            double_diffs.push(stop - start);
        }
    }

    let mut hat_diffs = Vec::with_capacity(N);
    for _ in 0..M {
        unsafe {
            let start = _rdtsc();
            let mut lst = HATResizableArray::new();
            for _ in 0..=N {
                lst.push_back(random::<usize>());
            }
            let stop = _rdtsc();
            hat_diffs.push(stop - start);
        }
    }

    (alpha_diffs, double_diffs, hat_diffs)
}


fn main() {
    // append, access, and scan
    let mut double: DoublingResizableArray<usize> = DoublingResizableArray::new();
    let dpush_latency = time_push(&mut double);
    let dget_latency = time_rd_access(&mut double);
    let dscan_latency = time_scan(&mut double);

    let mut hat: HATResizableArray<usize> = HATResizableArray::new();
    let hpush_latency = time_push(&mut hat);
    let hget_latency = time_rd_access(&mut hat);
    let hscan_latency = time_scan(&mut hat);

    let mut alpha: AlphaResizableArray<usize> = AlphaResizableArray::new();
    let apush_latency = time_push(&mut alpha);
    let aget_latency = time_rd_access(&mut alpha);
    let ascan_latency = time_scan(&mut alpha);


    // repeated overall inserts
    let (doverall_throughput, hoverall_throughput, aoverall_throughput) = time_overall();


    // writing results
    let path = get_project_root().unwrap().join("results_01.csv");
    let mut wtr = Writer::from_path(path).unwrap();
    let _res = wtr.write_record(&[
        "i",
        "double_push",
        "double_get",
        "double_scan",

        "hat_push",
        "hat_get",
        "hat_scan",

        "alpha_push",
        "alpha_get",
        "alpha_scan",
    ]);
    for i in 0..=N {
        let _res = wtr.write_record(&[
            i.to_string(),
            dpush_latency[i].to_string(),
            dget_latency[i].to_string(),
            dscan_latency[i].to_string(),

            hpush_latency[i].to_string(),
            hget_latency[i].to_string(),
            hscan_latency[i].to_string(),

            apush_latency[i].to_string(),
            aget_latency[i].to_string(),
            ascan_latency[i].to_string(),
        ]);
    }

    let path = get_project_root().unwrap().join("results_02.csv");
    let mut wtr = Writer::from_path(path).unwrap();
    let _res = wtr.write_record(&[
        "i",
        "double_overall",
        "hat_overall",
        "alpha_overall",
    ]);
    for i in 0..M {
        let _res = wtr.write_record(&[
            i.to_string(),
            doverall_throughput[i].to_string(),
            hoverall_throughput[i].to_string(),
            aoverall_throughput[i].to_string(),
        ]);
    }
}
