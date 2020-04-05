#include "path_tracer.h"
#include "terminal.h"

#include "vmath.h"

int main(int argc, char *argv[]) {
    terminal_color_t r;
    r.r = 255, r.g = 0, r.b = 0;
    terminal_color_t g;
    g.r = 0, g.g = 255, g.b = 0;
    terminal_color_t b;
    b.r = 0, b.g = 0, b.b = 255;
    terminal_color_t c1[2] = {r, g};
    terminal_color_t c2[2] = {g, b};
    uint32_t width = 180;
    uint32_t height = 100;
    uint32_t samples = 50;

    bool show_test_pattern = false;
    if (show_test_pattern) {
        terminal_clear();
        canvas_handler_t canvas = terminal_create_canvas(width, height);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (j % 2 == 0) {
                    terminal_set_pixel(canvas, i, j, c1[i % 2]);
                } else {
                    terminal_set_pixel(canvas, i, j, c2[i % 2]);
                }
            }
        }
        terminal_update(canvas);
    }

    // camera
    float aperture = 0.051;
    math::Vector3f lookfrom(10.0, 1.8, 2.4);
    math::Vector3f lookat(0.0, 0.0, 0.5);
    math::Vector3f up(0.0, 1.0, 0.0);
    float dist_to_focus = math::length(lookfrom - lookat);
    Camera camera(lookfrom, lookat, up, 30.0f, float(width) / height, aperture, dist_to_focus);

    // scene
    HitableList world = create_test_scene();

    // image
    Image image(width, height);

    // render the image
    for (int s = 0; s < samples; s++) {
        render_step(world, camera, image);
    }

    // show the image in the terminal
    terminal_clear();
    canvas_handler_t canvas = terminal_create_canvas(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            auto pixel = image.pixel_color(i,j);
            terminal_color_t c;
            c.r = pixel.r; c.g = pixel.g; c.b = pixel.b;
            terminal_set_pixel(canvas, i, height-j-1, c);
        }
    }
    terminal_update(canvas);

    printf("done\n");
    return 0;
}

/*
    let output_filename = matches.opt_str("o").unwrap_or(String::from("image.png"));
    let width = matches.opt_get_default::<u32>("w", 640).expect("invalid width");
    let height = matches.opt_get_default::<u32>("h", 360).expect("invalid heigh");
    let samples = matches.opt_get_default::<u32>("s", 10).expect("invalid number of samples");
    println!("sample path tracing. Rendering scene...");
    // create empty image
    let mut image = Image::new(width, height);
    // camera
    let aperture = 0.051;
    let lookfrom = Vec3::new(10.0, 1.8, 2.4);
    let lookat = Vec3::new(0.0, 0.0, 0.5);
    let up = Vec3::new(0.0,1.0,0.0);
    let dist_to_focus = (lookfrom-Vec3::new(4.0, 1.0, 0.0)).length();
    let camera = Camera::new(lookfrom, lookat, up,
                             30.0, (width as f32)/(height as f32),
                             aperture, dist_to_focus);
    // create scene
    let world = create_book_scene();
    // let world = create_test_scene();
    // create window with live framebuffer
    let mut buffer: Vec<u32> = vec![0; (width * height) as usize];
    let mut window = Window::new("Test - ESC to exit",
                                 width as usize,
                                 height as usize,
                                 WindowOptions::default()).unwrap_or_else(|e| { panic!("{}", e); });
    // progress bar
    let bar = ProgressBar::new(samples as u64);
    bar.set_style(indicatif::ProgressStyle::default_bar()
                  .template("[{elapsed}] [{wide_bar:.cyan/blue}] {pos}/{len} ({eta} rem.)")
                  .progress_chars("##-"));
    // render image and update  window
    if false {
        for _s in 0..samples {
            if window.is_open() && !window.is_key_down(Key::Escape) {
                render_step(&world, &camera, &mut image);
                // update framebuffer
                for j in 0..image.height {
                    for i in 0..image.width {
                        let (r,g,b) = image.val_rgb(i,j);
                        let rgb: u32 = 0xff << 24 | (r as u32) << 16 | (g as u32) << 8 | b as u32;
                        buffer[(i+j*image.width) as usize] = rgb;
                    }
                }
                window.update_with_buffer(&buffer).unwrap();
            }
            bar.inc(1);
        }
    } else {
        // create a vector with empty images
        println!("create array of image samples");
        let mut image_samples: Vec<Box<Image>> = Vec::new();
        for _i in 0..samples {
            image_samples.push(Box::new(Image::new(width,height)));
        }
        println!("render samples");
        // for mut img in image_samples.iter_mut() {

            {
        let thr1 = std::thread::spawn(|| {
            for mut img in &mut image_samples[..] {
                render_snapshot(&world, &camera, &mut img);
                bar.inc(1);
            }
        });
        thr1.join();
            }

        // render_snapshot(&world, &camera, &mut img);
        println!("combine samples");
        for img in image_samples.iter() {
            image.accumulate(&*img);
        }

    }
    bar.finish();

    println!("...Done!");
    // save image to file
    image.save(&output_filename);
}

*/
