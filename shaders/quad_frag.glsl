#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float brightness;
uniform int screenWidth;
uniform int screenHeight;
float r_off = 0.005;
float g_off = 0.02;
float b_off = 0.002;
void main(){
    vec2 fragCoord = gl_FragCoord.xy / vec2(screenWidth, screenHeight);
    vec2 center = vec2(0.5, 0.5); 
    float d = length(fragCoord - center);
    vec2 uv = TexCoords;

    uv = uv * 2.0f - 1.0f;
    vec2 offset = uv.yx / 5.0f;
    uv = uv + uv * offset * offset;
    uv = uv * 0.5f + 0.5f;

    vec4 originalColor = texture(screenTexture, uv);
    vec4 distortedColor = originalColor;

    vec2 r = vec2(d * r_off, 0.0); // Example offset for red
    vec2 g = vec2(d * g_off, 0.0); // Example offset for green
    vec2 b = vec2(d * b_off, 0.0); // Example offset for blue

    distortedColor.r =  texture(screenTexture, uv+r).r;
    distortedColor.g =  texture(screenTexture, uv+g).g;
    distortedColor.b =  texture(screenTexture, uv+b).b;

    originalColor = distortedColor;

    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);
    vec3 brightenedColor;

    if ((pixelCoords.x % 2 == 1) || (pixelCoords.y % 2 == 1)){
        brightenedColor = vec3(0.0, 0.0, 0.0); // Set the fragment color to black
    }
    else{
        // Increase the brightness of the non-black pixels
        brightenedColor = originalColor.rgb * 1.75;
    }
    if (uv.x <= 0.0f || 1.0f <= uv.x || uv.y <= 0.0f || 1.0f <= uv.y){
        originalColor.xyz *= 0;
    }

    uv = uv * 2.0f - 1.0f;
    vec2 v = 30 / vec2(screenWidth, screenHeight);
    v.x = smoothstep(0.0f, v.x, 1-abs(uv.x));
    v.y = smoothstep(0.0f, v.y, 1-abs(uv.y));
    v = clamp(v,0,1);
    brightenedColor.xyz *= v.x * v.y;

    FragColor = vec4(brightenedColor, 1.0);
}