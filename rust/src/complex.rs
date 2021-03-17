#[derive(Copy, Clone)]
pub(crate) struct Complex {
    real: f64,
    complex: f64,
}

impl Complex {
    pub fn new(real: f64, complex: f64) -> Complex {
        Complex { real, complex }
    }

    pub fn add(&self, other: &Complex) -> Complex {
        Complex::new(self.real + other.real, self.complex + other.complex)
    }

    pub fn product(&self, other: &Complex) -> Complex {
        Complex::new(
            self.real * other.real - self.complex * other.complex,
            self.real * other.complex + self.complex * other.real,
        )
    }

    pub fn modulus(&self) -> f64 {
        (self.real.powf(2.0) + self.complex.powf(2.0)).sqrt()
    }
}
