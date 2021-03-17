mod complex;

use crate::complex::Complex;
use image::{ImageBuffer, Luma};
use std::sync::{Arc, Mutex};
use std::thread;

extern crate num_cpus;

fn main() {
    //Get parameter from the arguments
    let side: usize = std::env::args()
        .last()
        .unwrap()
        .trim()
        .parse()
        .expect("Expected number");

    //Size of the vector
    let size = side * side;

    //Number of cpus available
    let cpus = num_cpus::get();

    //Elements per CPU to compute
    let batch_size = size / cpus;

    let mut handles = Vec::new();

    //Our data
    let matrix = Arc::new(Mutex::new(vec![255u8; size]));

    for batch in (0..size).step_by(batch_size) {
        let mut matrix = matrix.clone();

        //A new couroutine per CPU
        let handle = thread::spawn(move || {
            work(&mut matrix, batch, batch_size, side);
        });

        handles.push(handle);
    }

    //Wait for the rest of threads to finish
    for handle in handles {
        handle.join().unwrap();
    }

    //Create an image from vector and save it
    let image = ImageBuffer::<Luma<u8>, _>::from_vec(
        side as u32,
        side as u32,
        Arc::try_unwrap(matrix).unwrap().into_inner().unwrap(),
    )
    .unwrap();
    image.save("image.png").unwrap();
}

//Compute the elements of a flatened matrix of length size from batch to batch + batch_size
fn work(matrix: &mut Arc<Mutex<Vec<u8>>>, batch: usize, batch_size: usize, side: usize) {
    for i in batch..(batch + batch_size) {
        let value = iteration(&get_complex(
            (i % side) as i32,
            (i / side) as i32,
            side as i32,
        ));
        matrix.lock().unwrap()[i] = value;
    }
}

//Compute wheter a complex number belongs to the fractal(return 255) or not (slighly higher than 4)
fn iteration(cmp: &complex::Complex) -> u8 {
    let mut value = complex::Complex::new(0f64, 0f64);

    for step in 0..255 {
        value = value.product(&value).add(&cmp);

        if value.modulus() > 4.0 {
            return step as u8;
        }
    }

    255
}

//Get the complex number given coordinates in the plane
fn get_complex(x: i32, y: i32, size: i32) -> Complex {
    let coordinates: [f64; 2] = [
        (x as f64 + 0.5) / size as f64,
        (y as f64 + 0.5) / size as f64,
    ];
    Complex::new(
        (coordinates[0] - 0.5) * 2.0 - 1.0,
        (coordinates[1] - 0.5) * 2.0,
    )
}
