#version 330 core
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform sampler2D tex;

in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragUV;

out vec4 vertColor;

void main() {
  vec3 vertexPosition_worldspace = (M * fragPosition).xyz;

  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 EyeDirection_cameraspace = inverse(V)[3].xyz - vertexPosition_worldspace;
   
  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 LightPosition_cameraspace = (V * vec4(2, 2, 2, 1)).xyz;
  vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
   
  // Normal of the the vertex, in camera space
  vec3 Normal_cameraspace = (V * M * vec4(fragNormal, 1)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

  vec3 n = normalize(Normal_cameraspace);
  vec3 l = normalize(LightDirection_cameraspace);

  // Eye vector (towards the camera)
  vec3 E = normalize(EyeDirection_cameraspace);
  // Direction in which the triangle reflects the light
  vec3 R = reflect(-l, n);
  // Cosine of the angle between the Eye vector and the Reflect vector,
  // clamped to 0
  //  - Looking into the reflection -> 1
  //  - Looking elsewhere -> < 1
  float cosAlpha = clamp(dot(E, R), 0, 1);

  vec3 MaterialDiffuseColor = texture2D(tex, vec2(fragUV.x, fragUV.y)).rgb;
  vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;

  float distance = length(LightDirection_cameraspace);

  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

  float cosTheta = clamp(dot(n, l), 0, 1);
  vec3 color = MaterialDiffuseColor * lightColor * 5 * cosTheta / pow(distance, 2);

  color = lightColor * 2 * pow(cosAlpha, 5) / pow(distance, 2) + color;

  vertColor = vec4(MaterialAmbientColor + color, 1.0f);
}
