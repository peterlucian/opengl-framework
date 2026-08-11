#shader vertex
#version 330 core
     layout (location = 0) in vec2 aPos;   
     uniform mat4 u_MVP;
     void main()   
     {   
        gl_Position = u_MVP * vec4(aPos, 0.0, 1.0);
       
     };

#shader fragment
#version 330 core  
     layout (location = 0) out vec4 FragColor;
     
     void main()   
     {   
          //int index = int(v_TexIndex);
          gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
          
     };