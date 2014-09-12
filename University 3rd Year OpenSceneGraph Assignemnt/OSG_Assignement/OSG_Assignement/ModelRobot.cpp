#include "stdafx.h"
#include "ModelRobot.h"

ModelRobot::ModelRobot( string sFilePath, string sLimitsFilePath )
{
	_sFilePath = sFilePath;
	_sLimitsFilePath = sLimitsFilePath;
	_pmRotationNodes = new map<string, osg::MatrixTransform*>();
	_uiCurrentSelectedNode = 0;

	loadModel();
	parseRotationNodes();
}

ModelRobot::~ModelRobot()
{
	if( _pmRotationNodes ) delete _pmRotationNodes;
}

/*----------------------------------------------------------------------------------
Author: MaxAshton
function: parseRotationNodes
Description: 1. traverse to robot storing all nodes with _Rotator in a map with 
			    the full name as key
			 2. read robot_contorls file creating float array of rotation limits
			 3. find the node in the map, create its dataHolder and call back
----------------------------------------------------------------------------------*/
void ModelRobot::parseRotationNodes()
{
	//map traversals
	NodeFinder<osg::Node> nodeFinder( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	nodeFinder.traverse( *_pnRobotRoot );

	vector<osg::Node*>* pvAllNodes = nodeFinder.getList();
	
	for( vector<osg::Node*>::iterator i = pvAllNodes->begin(); i != pvAllNodes->end(); i++ )
	{
		string sName = ( *i )->getName();

		if( sName.length() > 8 )
		{
			if( sName.compare( sName.length() - 8, 8, "_Rotator" ) == 0 )
			{
				( *_pmRotationNodes )[sName] = ( osg::MatrixTransform* )( *i );
			}
		}
	}

	//read robot_control file
	ifstream isFile( _sLimitsFilePath );

	if( isFile.is_open() )
	{
		while( isFile.good() )
		{
			string sLine;
			getline( isFile, sLine );

			string sNodeName;
			float* pafLimits = new float[6];

			if( sLine.compare( 0, 14, "Rotation_Node:" ) == 0 )
			{
				//build the data
				sNodeName = sLine.substr( 14 );
				getline( isFile, sLine );
				pafLimits[0] = atof( sLine.substr( 4 ).c_str() );
				getline( isFile, sLine );
				pafLimits[1] = atof( sLine.substr( 4 ).c_str() );
				getline( isFile, sLine );
				pafLimits[2] = atof( sLine.substr( 4 ).c_str() );
				getline( isFile, sLine );
				pafLimits[3] = atof( sLine.substr( 4 ).c_str() );
				getline( isFile, sLine );
				pafLimits[4] = atof( sLine.substr( 4 ).c_str() );
				getline( isFile, sLine );
				pafLimits[5] = atof( sLine.substr( 4 ).c_str() );
			
				//find the node
				if( _pmRotationNodes->find( sNodeName ) != _pmRotationNodes->end() )
				{
					//create RobotMatrixDataStructure 
					osg::MatrixTransform* mT = ( *_pmRotationNodes )[sNodeName];
					buildNewRotationNodeStructure( mT );
					buildCallbackStrcture( pafLimits, mT );
				}
			}			
		}
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: buildNewRotationNodeStructure
Description: create structure allowing the node under control to be highlighted
----------------------------------------------------------------------------------*/
void ModelRobot::buildNewRotationNodeStructure( osg::MatrixTransform* pRotationNode )
{
	//switch
	osg::Switch* pSwitch = new osg::Switch();
	pSwitch->setName( "Switch:R:" + pRotationNode->getName() );
	
	//groups
	osg::Group* pgNoMaterial = new osg::Group();
	pgNoMaterial->setName( "GroupN:R:" + pRotationNode->getName() );
	pSwitch->addChild( pgNoMaterial );

	osg::Group* pgChangeMaterialGreen = new osg::Group();
	pgChangeMaterialGreen->setName( "GroupG:R:" + pRotationNode->getName() );
	pSwitch->addChild( pgChangeMaterialGreen );

	osg::Group* pgChangeMaterialRed = new osg::Group();
	pgChangeMaterialRed->setName( "GroupR:R:" + pRotationNode->getName() );
	pSwitch->addChild( pgChangeMaterialRed );
		
	//states
	osg::StateSet* pssMaterialStateGreen = pgChangeMaterialGreen->getOrCreateStateSet();
	pssMaterialStateGreen->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	
	osg::StateSet* pssMaterialStateRed = pgChangeMaterialRed->getOrCreateStateSet();
	pssMaterialStateRed->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );	

	//materials
	osg::Material* pmMovingColorGreen = new osg::Material();
	pmMovingColorGreen->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.0f, 1.0f, 0.0f, 1.0f ) );
	pmMovingColorGreen->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.0f, 1.0f, 0.0f, 1.0f ) );
	pmMovingColorGreen->setShininess( osg::Material::FRONT_AND_BACK, 1.0f );

	pssMaterialStateGreen->setAttributeAndModes( pmMovingColorGreen, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );	

	osg::Material* pmMovingColorRed = new osg::Material();
	pmMovingColorRed->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );	
	pmMovingColorRed->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.0f, 1.0f, 0.0f, 1.0f ) );
	pmMovingColorRed->setShininess( osg::Material::FRONT_AND_BACK, 1.0f );
	
	pssMaterialStateRed->setAttributeAndModes( pmMovingColorRed, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );	

	//get MatrixTransform children
	for( unsigned int i = 0; i < pRotationNode->getNumChildren(); i++ )
	{		
		osg::Geode* pChiled = dynamic_cast<osg::Geode*>( pRotationNode->getChild( i ) );
	
		//make sure its a geode
		if( pChiled != NULL )
		{
			//add geods to all groups
			pgNoMaterial->addChild( pChiled );
			pgChangeMaterialGreen->addChild( pChiled );
			pgChangeMaterialRed->addChild( pChiled );	

			//remove node from MatrixTransform
			pRotationNode->removeChild( pChiled );
		}					
	}	

	//add the switch to the MatrixTransform
	pRotationNode->addChild( pSwitch );
	
	//set color to gree to test
	pSwitch->setAllChildrenOff();
	pSwitch->setSingleChildOn( 0 );
} 
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: getModel
Description: return the models root node
----------------------------------------------------------------------------------*/
osg::Node* ModelRobot::getModel()
{
	return _pnRobotRoot;
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: loadModel
Description: 1. Create a dummy node so reuse of SceneManipulator::addSDFToScene is 
		     possible
			 2. Get dummy nodes first chiled (robot root)
		     3. discard dummy
----------------------------------------------------------------------------------*/
void ModelRobot::loadModel()
{
	osg::Group* pgHolder = new osg::Group();
	pgHolder->ref();

	osg::Matrixf matrix;
	matrix.makeRotate( 1.4f, 1.0f, 0.0f, 0.0f );

	SceneManipulator::addSDFToScene( pgHolder, "ROBOT", matrix, _sFilePath );

	_pnRobotRoot = pgHolder->getChild( 0 );
	_pnRobotRoot->ref();

	pgHolder->removeChild( _pnRobotRoot );	

	pgHolder->unref();
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: 
Description: 1. loop till finding the correct switch node
			 2. add callback and RobotRotationTransformDataHolder
----------------------------------------------------------------------------------*/
void ModelRobot::buildCallbackStrcture( float* pafLimits, osg::MatrixTransform* pRotationNode )
{
	//find switch; I didnt use a NodeVisitor here because theres no need to traverse all the childeren	
	osg::Switch* pSwitch = 0;

	for( unsigned int i = 0; i < pRotationNode->getNumChildren(); i++ )
	{
		osg::Node* pNode = pRotationNode->getChild( i );

		if( pNode->getName().compare( 0, 7, "Switch:" ) == 0 )
		{		
			pSwitch = dynamic_cast<osg::Switch*>( pNode );

			if( pSwitch )
			{
				//build RobotRotationDataHolder
				RobotRotationTransformDataHolder* pRRTDH = new RobotRotationTransformDataHolder( pafLimits, pSwitch, pRotationNode, 0.1f );

				//Attach to pRotationNode setUserData
				pRotationNode->setUserData( pRRTDH );

				//add callback to MT
				pRotationNode->addUpdateCallback( new TransformCallBack() );
			}	

			break;
		}
	}	
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: incrementSelection
Description: increment selected node, return to 0 if greater than node map size
----------------------------------------------------------------------------------*/
void ModelRobot::incrementSelection()
{
	_uiCurrentSelectedNode++;
	
	if( _uiCurrentSelectedNode >= _pmRotationNodes->size() )
	{
		_uiCurrentSelectedNode = 0;
	}
}
/*----------------------------------------------------------------------------------
Author: MaxAshton
function: setMovement
Description: 1. find the node based on index rather than key
		     2. set the movement in RobotRotationTransformDataHolder based on 
				parameter
----------------------------------------------------------------------------------*/
void ModelRobot::setMovement( unsigned int uiDirection )
{
	osg::MatrixTransform* pNode = 0;

	unsigned int uiCount = 0; 
	for( map<string, osg::MatrixTransform*>::iterator i = _pmRotationNodes->begin(); i != _pmRotationNodes->end(); i++, uiCount++ ) 
	{
		if( uiCount == _uiCurrentSelectedNode )
		{
			pNode = i->second;
		}
	}


	RobotRotationTransformDataHolder* pData = dynamic_cast<RobotRotationTransformDataHolder*> ( pNode->getUserData() );

	if( pData != NULL )
	{
		pData->setMovement( uiDirection );
	}
}
