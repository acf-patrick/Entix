#include "specializations.h"

YAML::Emitter &operator<<(YAML::Emitter &out, const SDL_Rect &rect) {
    out << YAML::Flow;
    out << YAML::BeginSeq;
    out << YAML::Flow << YAML::BeginSeq << rect.x << rect.y << YAML::EndSeq;
    out << YAML::Flow << YAML::BeginSeq << rect.w << rect.h << YAML::EndSeq;
    out << YAML::EndSeq;
    return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const SDL_Color &color) {
    out << YAML::Flow;
    out << YAML::BeginSeq;
    out << (int)color.r << (int)color.g << (int)color.b << (int)color.a;
    out << YAML::EndSeq;
    return out;
}
