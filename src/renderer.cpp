#include "includes/renderer.hpp"

RectangleRenderer::RectangleRenderer(sf::Color color, sf::Vector2f size) :
    rect(size)
{
    rect.setFillColor(color);
}

RectangleRenderer::RectangleRenderer(sf::Vector2f size) :
    RectangleRenderer(sf::Color::White, size)
{}

RectangleRenderer::RectangleRenderer(const RectangleRenderer& other) :
    rect(other.rect)
{}

void RectangleRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(rect, states);
}

void RectangleRenderer::setColor(sf::Color color) {
    rect.setFillColor(color);
}

SpriteRenderer::SpriteRenderer(std::string texturePath) :
    SpriteRenderer(texturePath, sf::Transform())
{}

SpriteRenderer::SpriteRenderer(std::string texturePath, sf::Transform localTransform) :
    localTransform(localTransform)
{
    texture = new sf::Texture();
    texture->loadFromFile(texturePath);
    sprite.setTexture(*texture);
}

SpriteRenderer::SpriteRenderer(const SpriteRenderer& other) :
    localTransform(other.localTransform),
    texture(other.texture)
{
    sprite.setTexture(*texture);
}

void SpriteRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= localTransform;
    target.draw(sprite, states);
}
