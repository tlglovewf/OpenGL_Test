//
//  Shader.fsh
//  GLSLTest
//
//  Created by Zenny Chen on 4/11/10.
//  Copyright GreenGames Studio 2010. All rights reserved.
//

varying vec4 colorVarying;

void main()
{
	gl_FragColor = colorVarying;
    gl_FragColor.r = 1.0;
}
