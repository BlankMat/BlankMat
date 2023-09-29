#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS
{
	vec3 geoVertColor;
	float vertExcludeEdge;
} gs_in[];

out GS
{
	vec3 VertColor;
	vec3 dist;
} gs_out;

uniform mat4 MVP;
uniform vec2 WIN_SCALE;

void main()
{
	float MEW = 100.0;
	// taken from 'Single-Pass Wireframe Rendering'
	vec2 p0 = WIN_SCALE * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
	vec2 p1 = WIN_SCALE * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
	vec2 p2 = WIN_SCALE * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;
	vec2 v0 = p2-p1;
	vec2 v1 = p2-p0;
	vec2 v2 = p1-p0;
	float area = abs(v1.x*v2.y - v1.y * v2.x);

	float res1 = clamp(gs_in[0].vertExcludeEdge + gs_in[1].vertExcludeEdge, 0, 1);
	float res2 = clamp(gs_in[0].vertExcludeEdge + gs_in[2].vertExcludeEdge, 0, 1);
	gs_out.dist = vec3(area/length(v0),res1*MEW,res2*MEW);
	gl_Position = gl_in[0].gl_Position;
	gs_out.VertColor = gs_in[0].geoVertColor;
	EmitVertex();
	
	res1 = clamp(gs_in[1].vertExcludeEdge + gs_in[0].vertExcludeEdge, 0, 1);
	res2 = clamp(gs_in[1].vertExcludeEdge + gs_in[2].vertExcludeEdge, 0, 1);
	gs_out.dist = vec3(res1*MEW,area/length(v1),res2*MEW);
	gl_Position = gl_in[1].gl_Position;
	gs_out.VertColor = gs_in[1].geoVertColor;
	EmitVertex();
	
	res1 = clamp(gs_in[2].vertExcludeEdge + gs_in[0].vertExcludeEdge, 0, 1);
	res2 = clamp(gs_in[2].vertExcludeEdge + gs_in[1].vertExcludeEdge, 0, 1);
	gs_out.dist = vec3(res1*MEW,res2*MEW,area/length(v2));
	gs_out.VertColor = gs_in[2].geoVertColor;
	gl_Position = gl_in[2].gl_Position;
	
	EmitVertex();
	EndPrimitive();
}