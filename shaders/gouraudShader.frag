#version 330 core

in GS
{
	vec3 VertColor;
	vec3 dist;
};

out vec4 FragColor;

void main()
{
	vec3 dist_vec = dist;
	float d = min(dist_vec[0], min(dist_vec[1], dist_vec[2]));
	float I = exp2(-2.0*d*d);

	vec3 result = I*(vec3(1.0, 0.372, 0.122)) + (1.0 - I)*VertColor;

    FragColor = vec4(result, 1.0);
}