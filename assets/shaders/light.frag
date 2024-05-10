#version 330 core

#define MAX_LIGHT_COUNT 16
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
   int type;
   vec3 color;
   vec3 position;
   vec3 direction;
   vec3 attenuation;
   vec2 cone_angles;
};

struct Material {
   sampler2D albedo;
   sampler2D specular;
   sampler2D ambient_occlusion;
   sampler2D roughness;
   sampler2D emissive;
  
};

uniform Material material;
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;

in Varyings
{
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fsin;


out vec4 frag_color;

void main() {
   vec3 normal = normalize(fsin.normal);
   vec3 view = normalize(fsin.view);
   int count = min(light_count, MAX_LIGHT_COUNT);
   vec3 mat_diffuse =texture(material.albedo, fsin.tex_coord).rgb;
   vec3 mat_specular = texture(material.specular, fsin.tex_coord).rgb;
   vec3 mat_emissive =  texture(material.emissive, fsin.tex_coord).rgb;
   vec3 mat_ambient = mat_diffuse * texture(material.ambient_occlusion,fsin.tex_coord).r;
   float roughness = texture(material.roughness, fsin.tex_coord).r;
   float mat_shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;

   vec3 accumulated_light=mat_ambient+mat_emissive;
   for(int index = 0; index < count; index++){
      Light light = lights[index];
      vec3 light_direction;
      float attenuation = 1;
      if(light.type == DIRECTIONAL)
         light_direction = -light.direction;
      else {
         light_direction = light.position-fsin.world;
         float dist = length(light_direction);
         light_direction /= dist;
         attenuation *= 1.0f / dot(light.attenuation, vec3(1.0, dist, dist*dist));
         if(light.type == SPOT){
            float angle = acos(dot(light.direction, -light_direction));
            attenuation *= smoothstep(light.cone_angles[1], light.cone_angles[0], angle);
         }
      }
      vec3 reflected = reflect(-light_direction, normal);
      float lambert = max(0.0f, dot(normal, light_direction));
      float phong = pow(max(0.0f, dot(view, reflected)), mat_shininess);
      vec3 diffuse = mat_diffuse * light.color * lambert;
      vec3 specular = mat_specular * light.color * phong;
      accumulated_light += (diffuse + specular) * attenuation ;
   }
   frag_color =  vec4(accumulated_light, 1.0f);

}
