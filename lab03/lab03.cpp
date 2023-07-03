#include "lab03.h"

using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

vector<double> input_numbers(size_t count) {
    cerr << "Input numbers:" << '\n';
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        cin >> result[i];
    }
    return result;
}

pair<double, double> find_minmax(const vector<double>& numbers) {
    double min = numbers[0];
    double max = numbers[0];
    for (double number : numbers) {
        if (number < min) {
            min = number;
        }
        else if (number > max) {
            max = number;
        }
    }
    return pair<double, double>(min, max);
}

vector<size_t> make_histogram(const vector<double>& numbers, size_t bin_count) {
    vector<size_t> result(bin_count);

    auto [min, max] = find_minmax(numbers);

    double bin_size = (max - min) / bin_count;

    for (size_t i = 0; i < numbers.size(); i++) {
        bool found = false;
        for (size_t j = 0; (j < bin_count - 1) && !found; j++) {
            auto lo = min + j * bin_size;
            auto hi = min + (j + 1) * bin_size;
            if ((lo <= numbers[i]) && (numbers[i] < hi)) {
                result[j]++;
                found = true;
            }
        }

        if (!found) {
            result[bin_count - 1]++;
        }
    }

    return result;
}

void svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text) {
    cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke, string fill) {
    cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' stroke='" << stroke << "' fill='" << fill << "' />";
}

void show_histogram_svg(const vector<size_t>& bins, const vector<string>& colors) {
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_TOP = 20;
    const auto BIN_WIDTH = 20;
    const auto BIN_HEIGHT = 10;

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

    size_t total_count = 0;
    for (size_t bin : bins) {
        total_count += bin;
    }

    size_t max_percentage = 100;
    size_t bin_index = 0;
    for (size_t bin : bins) {
        double bin_width = (static_cast<double>(bin) / total_count) * IMAGE_WIDTH;
        double percentage = (static_cast<double>(bin) / total_count) * max_percentage;

        string bar_color = colors[bin_index % colors.size()];

        svg_rect(TEXT_LEFT + 20, TEXT_TOP + bin_index * (BIN_HEIGHT + 5), bin_width, BIN_HEIGHT, "black", bar_color);

        std::ostringstream oss;
        oss << bin;

        svg_text(TEXT_LEFT, TEXT_TOP + bin_index * (BIN_HEIGHT + 5) + BIN_HEIGHT - 2, oss.str() + "|");

        oss.str("");
        if (percentage == 0.0) {
            oss << "0%";
        }
        else {
            oss << setw(2) << setfill('0') << std::round(percentage) << "%";
        }
        svg_text(TEXT_LEFT + bin_width + 30, TEXT_TOP + bin_index * (BIN_HEIGHT + 5) + BIN_HEIGHT - 2, oss.str());

        bin_index++;
    }

    svg_end();
}

int main() {
    size_t number_count;

    cerr << "Input the count of numbers: ";
    cin >> number_count;

    const auto numbers = input_numbers(number_count);

    size_t bin_count;

    cerr << "Input the count of bins: ";
    cin >> bin_count;

    const auto bins = make_histogram(numbers, bin_count);

    vector<string> colors = { "red", "blue", "green", "yellow", "purple", "orange" };

    show_histogram_svg(bins, colors);

    return 0;
}
