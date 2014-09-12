#include "stdafx.h"
#include "SceneManipulator.h"

SceneManipulator::SceneManipulator()
{	
}

SceneManipulator::~SceneManipulator()
{

}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addObjectToScene
description: uses a switch to determine how to add the object passing different parameters
			 in to two different function addSDFToScene or addGeometryToScene
----------------------------------------------------------------------------------*/
void SceneManipulator::addObjectToScene( osg::Group* pnScene, unsigned int uiObject, string sName, string sTexture, osg::Matrixf mPosition )
{
	switch ( uiObject )
	{
		case SCENEFILE: addSDFToScene( pnScene, sName, mPosition, sTexture ); break;
		case SPHERE:    addGeometryToScene( pnScene, sName, sTexture, mPosition, ShapeFactory::instance().getSphere() ); break; 
		case PHYRAMID:  addGeometryToScene( pnScene, sName, sTexture, mPosition, ShapeFactory::instance().getPyramid() ); break;
	    case RECTANGLE: addGeometryToScene( pnScene, sName, sTexture, mPosition, ShapeFactory::instance().getRectangle( STATICVALS::SQUARE_DIM(), STATICVALS::SQUARE_DIM() ) ); break;
		case BOX:       addGeometryToScene( pnScene, sName, sTexture, mPosition, ShapeFactory::instance().getBox() ); break;
	}	
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addGeometryToScene
description: calls buildManipulationStructure to generate basic group node structure
			 , if there is a texture add it to the groups data holder, load the
			 texture if needed updating the groups set state, finish by adding the 
			 geometry to the MatrixTransform.
----------------------------------------------------------------------------------*/
void SceneManipulator::addGeometryToScene( osg::Group* pnScene, string sName, string sTexture, osg::Matrixf mPosition, osg::Geode* pnGeometry )
{
	cout << "Info: Added Object: Geometry; Name:" << sName << "\n";	

	osg::Group* pGroup = buildManipulationStructure( sName, mPosition );
	
	GroupDataHolder* pGHolder = new GroupDataHolder();
	pGroup->setUserData( pGHolder );

	pnGeometry->setName( STATICVALS::GM_MO + sName );

	//if texture load it
	if( sTexture.compare( "" ) != 0 )
	{
		osg::StateSet* pState = loadTexture( sTexture );
		pGroup->setStateSet( pState );
		pGHolder->TextureLocation = sTexture;
	}
	
	osg::MatrixTransform* pMatrixTransform = ( osg::MatrixTransform* ) pGroup->getChild( 1 );
	pMatrixTransform->addChild( pnGeometry );

	pnScene->addChild( pGroup );
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: buildManipulationStructure
description: Create a MatrixTransfrom and then pass it to buildManipulationStructureUsingMT
----------------------------------------------------------------------------------*/
osg::Group* SceneManipulator::buildManipulationStructure( string sName, osg::Matrixf mPosition )
{
	osg::Matrixf o2Mat;
	o2Mat.makeTranslate( 0.0f, 0.0f, -10.0f );

	osg::Matrixf mRotate;
	mRotate.makeRotate( -1.4, 1.0f, 0.0f, 0.0f );

	osg::MatrixTransform* pMatrixTransform = new osg::MatrixTransform();
	pMatrixTransform->setName( STATICVALS::MT_MO + sName );
	pMatrixTransform->setMatrix( mRotate* o2Mat * mPosition );		

	return buildManipulationStructureUsingMT( sName, pMatrixTransform );
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: buildManipulationStructureUsingMT
description: Create dragger structure group->switch->dragers, group->MatrixTransfrom
			 and add call backs and dataHolders to the MatrixTransform to handle animation
----------------------------------------------------------------------------------*/
osg::Group* SceneManipulator::buildManipulationStructureUsingMT( string sName, osg::MatrixTransform* pmtPosition )
{
	osg::Group* pGroup = new osg::Group();
	pGroup->setName( STATICVALS::G_MO + sName );

	osg::Switch* pSwitch = new osg::Switch();
	pSwitch->setName( STATICVALS::S_MO + sName );

	osg::StateSet* pStateSet = pSwitch->getOrCreateStateSet();
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );

	pSwitch->setAllChildrenOff();

	pGroup->addChild( pSwitch );	

	osg::Matrixf o2Mat;
	o2Mat.makeTranslate( 0.0f, 0.0f, -10.0f );

	osg::Matrixf mRotate;
	mRotate.makeRotate( -1.4, 1.0f, 0.0f, 0.0f );

	pGroup->addChild( pmtPosition );

	//add callback and data holder
	TransformDataHolder* pDataHolder = new TransformDataHolder( pmtPosition, STATICVALS::GLOBAL_MOVE_SPEED() );
	pmtPosition->setUserData( pDataHolder );
	pmtPosition->setUpdateCallback( new TransformCallBack() );

	osg::Matrixf mScales;
	mScales.makeScale( STATICVALS::DRAGER_SCALE(), STATICVALS::DRAGER_SCALE(), STATICVALS::DRAGER_SCALE() );

	addDragger( new osgManipulator::TranslateAxisDragger, pSwitch, pmtPosition, mScales );
	addDragger( new osgManipulator::TrackballDragger, pSwitch, pmtPosition, mScales );
	addDragger( new osgManipulator::TabBoxDragger, pSwitch, pmtPosition, mScales );

	addSelectionStructure( sName, pmtPosition );

	return pGroup;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addDragger
description: Set up basic dragger config, scale matrix up so it appears bigger than object
			 , control key set
----------------------------------------------------------------------------------*/
void SceneManipulator::addDragger( osgManipulator::Dragger* pDragger, osg::Switch* pSwitch, osg::MatrixTransform* pMatrixTransform, osg::Matrix mScale )
{
	pDragger->setupDefaultGeometry();
	pDragger->setHandleEvents( true );
	pDragger->addTransformUpdating( pMatrixTransform );
	pDragger->setActivationModKeyMask( osgGA::GUIEventAdapter::MODKEY_CTRL );
	pDragger->setMatrix( mScale * pMatrixTransform->getMatrix() );
	pSwitch->addChild( pDragger );
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: loadTexture
description: use osg to load a texture and assign it to a SS, returns an new SS
			 but can also be used to update a previous one by passing it in as a 
			 parameter
----------------------------------------------------------------------------------*/
osg::StateSet* SceneManipulator::loadTexture( string sName, osg::StateSet* pssToChange )
{
	// protect from being optimized away as static state:
	osg::Texture2D* pTexture = new osg::Texture2D;	
	pTexture->setDataVariance( osg::Object::DYNAMIC ); 

	// load an image by reading a file: 
	osg::Image* pImage = osgDB::readImageFile( sName );
	pTexture->setImage( pImage );

	osg::StateSet* pState = new osg::StateSet();
	pState->setTextureAttributeAndModes( 0, pTexture, osg::StateAttribute::ON );

	if( pssToChange != NULL )
	{
		pssToChange->setTextureAttributeAndModes( 0, pTexture, osg::StateAttribute::ON );
	}

	return pState;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addSDFToScene
description: use osg to load a .osg file and the add the files root node to
			 existing group structure, print out the structure after loading
----------------------------------------------------------------------------------*/
void SceneManipulator::addSDFToScene( osg::Group* pnScene, string sName, osg::Matrixf mPosition, string sFileName )
{
	cout << "Info: Added Object: OSGFIle; Name:" << sName << "\n";	

	osg::Group* pGroup = buildManipulationStructure( sName, mPosition );
	pGroup->setUserData( new GroupDataHolder() );

	osg::Node* pFileNode = osgDB::readNodeFile( sFileName );

	cout << "\n";
	cout << "---- " << sFileName << " File Structure----" << "\n";

	NodePrinter<osg::Node> nodePrinter( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	nodePrinter.traverse( *pFileNode );
	
	if( pFileNode != NULL )
	{
		pFileNode->setName( STATICVALS::GM_MO + sName );

		osg::MatrixTransform* pMatrixTransform = ( osg::MatrixTransform* ) pGroup->getChild( 1 );
		pMatrixTransform->addChild( pFileNode );

		pnScene->addChild( pGroup );
	}
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addMySDFToScene
description: use osg to load a scene file I created, difference here being object 
			 position will be saved into the file already so no need to set a 
			 matrix
----------------------------------------------------------------------------------*/
osg::Group* SceneManipulator::addMySDFToScene( osg::Group* pnScene, string sName, string sFileName, string sTexture )
{
	cout << "Info: Added Object: OSGFIle; Name:" << sName << "\n";	
	
	osg::MatrixTransform* pFileNode = ( osg::MatrixTransform* ) osgDB::readNodeFile( sFileName );

	if( pFileNode != NULL )
	{
		pFileNode->setName( STATICVALS::GM_MO + sName );

		osg::Group* pGroup = buildManipulationStructureUsingMT( sName, pFileNode );

		if( sTexture.compare( "" ) != 0 )
		{
			GroupDataHolder* pGHolder = new GroupDataHolder();
			pGroup->setUserData( pGHolder );
			
			osg::StateSet* pState = pGroup->getOrCreateStateSet();
			loadTexture( sTexture, pState );
			pGroup->setStateSet( pState );
			pGHolder->TextureLocation = sTexture;
		}

		pnScene->addChild( pGroup );

		return pGroup;
	}

	return NULL;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: addSelectionStructure
description: add selection structure MatrixTransform, MT->objectGeode 
             MT->switch->select sphere geodes
----------------------------------------------------------------------------------*/
void SceneManipulator::addSelectionStructure( string sName, osg::MatrixTransform* pMT )
{
	//look for existing structure
	NodeFinder<osg::Switch> nodeFinder( osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN );
	nodeFinder.traverse( *pMT );

	if( nodeFinder.getFirst() != NULL )
	{
		if( nodeFinder.getFirst()->getName().compare( 0, 10, STATICVALS::S_SO ) == 0 )
		{
			//already has it
			nodeFinder.getFirst()->setAllChildrenOff();
			return;
		}
	}

	osg::Switch* pSwitch = new osg::Switch();
	pSwitch->setName( STATICVALS::S_SO + sName );
	
	osg::Geode* pGeodeMove = ShapeFactory::instance().getWireframeSphere( STATICVALS::WIREFRAME_SPHERE_RADIOUS(), osg::Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );
	pGeodeMove->setName( STATICVALS::GEO_SO_M + sName );
	pSwitch->addChild( pGeodeMove );

	osg::Geode* pGeodeRotate = ShapeFactory::instance().getWireframeSphere( STATICVALS::WIREFRAME_SPHERE_RADIOUS(), osg::Vec4f( 0.0f, 1.0f, 0.0f, 1.0f ) );
	pGeodeRotate->setName( STATICVALS::GEO_SO_R + sName );
	pSwitch->addChild( pGeodeRotate );

	osg::Geode* pGeodeScale = ShapeFactory::instance().getWireframeSphere( STATICVALS::WIREFRAME_SPHERE_RADIOUS(), osg::Vec4f( 0.0f, 0.0f, 1.0f, 1.0f ) );
	pGeodeScale->setName( STATICVALS::GEO_SO_S + sName );
	pSwitch->addChild( pGeodeScale );
	
	pMT->addChild( pSwitch );

	pSwitch->setAllChildrenOff();
}