#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec2 u_TexScale;
uniform vec2 u_TexOffset;
uniform bool u_Mirror;

void main(){
	gl_Position = u_Projection*u_View*u_Model*position;
	//gl_Position = position;
	if(u_Mirror){
		v_TexCoord = vec2((1 - position.x)*u_TexScale.x+u_TexOffset.x,position.y*u_TexScale.y+u_TexOffset.y);
	}
	else{
		v_TexCoord = vec2(position.x*u_TexScale.x+u_TexOffset.x,position.y*u_TexScale.y+u_TexOffset.y);
	}
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//vec4 texColor = vec4(1,0,0,1);
	color = texColor;
}