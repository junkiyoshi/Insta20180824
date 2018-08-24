#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(30);
	this->box2d.registerGrabbing();

	this->number_of_targets = 80;

	for (int i = 0; i < this->number_of_targets; i++) {

		this->circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		this->circles.back().get()->setPhysics(1.5, 0.5, 0.1);
		this->circles.back().get()->setup(this->box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 15);

		glm::vec4  color = glm::vec4(1.0, 1.0, 1.0, 0.0);
		int color_type = ofRandom(6);
		switch (color_type) {
		case 0:
			color = glm::vec4(1.0, 0.0, 0.0, 0.0);
			break;
		case 1:
			color = glm::vec4(0.0, 1.0, 0.0, 0.0);
			break;
		case 2:
			color = glm::vec4(0.0, 0.0, 1.0, 0.0);
			break;
		case 3:
			color = glm::vec4(1.0, 1.0, 0.0, 0.0);
			break;
		case 4:
			color = glm::vec4(0.0, 1.0, 1.0, 0.0);
			break;
		case 5:
			color = glm::vec4(1.0, 0.0, 1.0, 0.0);
			break;
		}
		this->colors.push_back(color);
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	float direction = ofGetFrameNum() % 45 == 0 ? 2 : -1.5;

	for (int i = 0; i < this->circles.size(); i++) {

		for (int j = i + 1; j < this->circles.size(); j++) {

			float distance = this->circles[i]->getPosition().distance(this->circles[j]->getPosition());
			if (distance < 80) {

				this->circles[i]->addForce(this->circles[i]->getPosition() - this->circles[j]->getPosition(), 1.0 * direction);
				this->circles[j]->addForce(this->circles[j]->getPosition() - this->circles[i]->getPosition(), 1.0 * direction);
			}
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec4> points;
	for (int i = 0; i < this->number_of_targets; i++) {

		glm::vec4 p = glm::vec4(this->circles[i]->getPosition().x, this->circles[i]->getPosition().y, 0, 0);
		points.push_back(p);
	}

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform4fv("targets", &points[0].x, this->number_of_targets);
	this->shader.setUniform4fv("colors", &this->colors[0].x, this->number_of_targets);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}