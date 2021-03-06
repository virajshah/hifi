//
//  Light.cpp
//  libraries/model/src/model
//
//  Created by Sam Gateau on 1/26/2014.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
#include "Light.h"

using namespace model;

Light::Light() :
    _flags(0),
    _schemaBuffer(),
    _transform() {
    // only if created from nothing shall we create the Buffer to store the properties
    Schema schema;
    _schemaBuffer = gpu::BufferView(new gpu::Buffer(sizeof(Schema), (const gpu::Buffer::Byte*) &schema));
}

Light::Light(const Light& light) :
    _flags(light._flags),
    _schemaBuffer(light._schemaBuffer),
    _transform(light._transform) {
}

Light& Light::operator= (const Light& light) {
    _flags = (light._flags);
    _schemaBuffer = (light._schemaBuffer);
    _transform = (light._transform);

    return (*this);
}

Light::~Light() {
}

void Light::setPosition(const Vec3& position) {
    _transform.setTranslation(position);
    editSchema()._position = Vec4(position, 1.f);
}

void Light::setOrientation(const glm::quat& orientation) {
    _transform.setRotation(orientation);
}

void Light::setDirection(const Vec3& direction) {
    editSchema()._direction = glm::normalize(direction);
}

const Vec3& Light::getDirection() const {
    return getSchema()._direction;
}

void Light::setColor(const Color& color) {
    editSchema()._color = color;
}

void Light::setIntensity(float intensity) {
    editSchema()._intensity = intensity;
}

void Light::setMaximumRadius(float radius) {
    if (radius <= 0.f) {
        radius = 1.0f;
    }
    float CutOffIntensityRatio = 0.05f;
    float surfaceRadius = radius / (sqrt(1.0f / CutOffIntensityRatio) - 1.f);
    editSchema()._attenuation = Vec4(surfaceRadius, 1.0f/surfaceRadius, CutOffIntensityRatio, radius);
}

void Light::setSpotAngle(float angle) {
    if (angle <= 0.f) {
        angle = 0.0f;
    }
    float cosAngle = cos(angle);
    editSchema()._spot.x = cos(angle);
    editSchema()._spot.y = sin(angle);
    editSchema()._spot.z = angle;
}

void Light::setSpotExponent(float exponent) {
    if (exponent <= 0.f) {
        exponent = 1.0f;
    }
    editSchema()._spot.w = exponent;
}

void Light::setShowContour(float show) {
    if (show <= 0.f) {
        show = 0.0f;
    }
    editSchema()._control.w = show;
}
