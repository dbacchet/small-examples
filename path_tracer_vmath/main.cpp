#include "path_tracer.h"
#include "terminal.h"
#include "cmdline.h"

#include "vmath.h"

void view_image_terminal(const Image &image) {
    terminal_clear();
    canvas_handler_t canvas = terminal_create_canvas(image.width, image.height);
    for (uint32_t i = 0; i < image.width; i++) {
        for (uint32_t j = 0; j < image.height; j++) {
            auto pixel = image.pixel_color(i, j);
            terminal_color_t c;
            c.r = pixel.r;
            c.g = pixel.g;
            c.b = pixel.b;
            terminal_set_pixel(canvas, i, image.height - j - 1, c);
        }
    }
    terminal_update(canvas);
}

int main(int argc, char *argv[]) {
    cmdline::parser parser;
    parser.add<uint32_t>("width", 'w', "image width", false, 150);
    parser.add<uint32_t>("height", 'h', "image height", false, 100);
    parser.add<uint32_t>("samples", 's', "path tracing samples per pixel", false, 30);
    parser.parse_check(argc, argv);
    uint32_t width = parser.get<uint32_t>("width");
    uint32_t height = parser.get<uint32_t>("height");
    uint32_t samples = parser.get<uint32_t>("samples");

    // camera
    float aperture = 0.051;
    math::Vector3f lookfrom(10.0, 1.8, 2.4);
    math::Vector3f lookat(0.0, 0.0, 0.5);
    math::Vector3f up(0.0, 1.0, 0.0);
    float dist_to_focus = math::length(lookfrom - lookat);
    Camera camera(lookfrom, lookat, up, 30.0f, float(width) / height, aperture, dist_to_focus);

    // scene
    bool show_test_scene = false;
    HitableList world = show_test_scene ? create_test_scene() : create_book_scene();

    // image
    Image image(width, height);

    // render the image
    for (uint32_t s = 0; s < samples; s++) {
        render_step(world, camera, image);
        view_image_terminal(image);
        printf("computed sample %d/%d\n", s + 1, samples);
    }

    // show the image in the terminal

    printf("done\n");
    return 0;
}

