#include <algorithm>
#include <string>

#include "Engine/Point.hpp"
#include "Slider.hpp"
#include <allegro5/bitmap_draw.h>

#include "Engine/LOG.hpp"
#include "UI/Component/ImageButton.hpp"

Slider::Slider(float x, float y, float w, float h)
    : ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
      Bar("stage-select/bar.png", x, y, w, h),
      End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
      End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
    Position.x += w;
    Position.y += h / 2;
    Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw() const {
    // DONE HACKATHON-5 (3/4): The slider's component should be drawn here.
    // Draw the other components for slider
    Bar.Draw();
    End1.Draw();
    End2.Draw();
    // Handle
    Engine::ImageButton::Draw();
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(float value) {
    // DONE HACKATHON-5 (4/4): Set the value of the slider and call the callback.
    Position.x = End1.Position.x + value * Bar.Size.x;

    if (OnValueChangedCallback)
        OnValueChangedCallback(value);
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn)
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
    ImageButton::OnMouseMove(mx, my);
    if (Down) {
        // Clamp
        float clamped = std::min(std::max(static_cast<float>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
        float value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
        SetValue(value);
    }
}
