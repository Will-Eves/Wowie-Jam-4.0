#version 410 core

in vec2 albedoCoordinate;
in vec2 fragmentPosition;

struct UI {
	vec4 color;

	sampler2D image;
	int hasImage;
	int isText;

	vec2 minBound;
	vec2 maxBound;
	float radius;
};

uniform UI ui;

layout(location = 0) out vec4 color;

void main() {
	color = ui.color;
	
	if (ui.hasImage == 1) {
		if (ui.isText == 0) color *= texture2D(ui.image, albedoCoordinate);
		else                color.w *= texture2D(ui.image, albedoCoordinate).a;
	}

	if (ui.isText == 0) {
		int inCorner = 0;

		inCorner += (fragmentPosition.x < ui.minBound.x + ui.radius && fragmentPosition.y > ui.maxBound.y - ui.radius ? 1 : 0);
		inCorner += (fragmentPosition.x < ui.minBound.x + ui.radius && fragmentPosition.y < ui.minBound.y + ui.radius ? 1 : 0);
		inCorner += (fragmentPosition.x > ui.maxBound.x - ui.radius && fragmentPosition.y > ui.maxBound.y - ui.radius ? 1 : 0);
		inCorner += (fragmentPosition.x > ui.maxBound.x - ui.radius && fragmentPosition.y < ui.minBound.y + ui.radius ? 1 : 0);

		vec2 topleft = vec2(ui.minBound.x + ui.radius, ui.maxBound.y - ui.radius);
		float topleftDistance = length(fragmentPosition - topleft);
		vec2 topright = vec2(ui.maxBound.x - ui.radius, ui.maxBound.y - ui.radius);
		float toprightDistance = length(fragmentPosition - topright);
		vec2 bottomleft = vec2(ui.minBound.x + ui.radius, ui.minBound.y + ui.radius);
		float bottomleftDistance = length(fragmentPosition - bottomleft);
		vec2 bottomright = vec2(ui.maxBound.x - ui.radius, ui.minBound.y + ui.radius);
		float bottomrightDistance = length(fragmentPosition - bottomright);

		float minDistance = min(min(topleftDistance, toprightDistance), min(bottomleftDistance, bottomrightDistance));

		if (minDistance > ui.radius && inCorner > 0) discard;
	}

	if (color.w == 0.0) discard;
}