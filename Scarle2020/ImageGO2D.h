#ifndef _IMAGE_GO_2D_H_
#define _IMAGE_GO_2D_H_
#include "GameObject2D.h"

//=================================================================
//A Game Object 2D for displaying images
//image needs to have been converted to a dds by the Assets project
//add orginal file to this project and set it to "Image Content Pipeline"
// in a similar fashion as getting the models
//=================================================================

class ImageGO2D :public GameObject2D
{
public:
	ImageGO2D(string _fileName, ID3D11Device* _GD);
	virtual ~ImageGO2D();
	ImageGO2D(const ImageGO2D& orig);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);
	virtual void OnCollision(GameData* _GD, GameObject2D* _other);

	void SetSprite(string _fileName, ID3D11Device* _GD);

	ID3D11ShaderResourceView* GetTexture() { return m_pTextureRV; }
	string GetFileName() { return m_filename; }

protected:

	ID3D11ShaderResourceView* m_pTextureRV;
	string m_filename; //for cloning
};

#endif