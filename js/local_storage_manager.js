window.fakeStorage = {
    _data: {},

    setItem: function (id, val) {
        return this._data[id] = String(val);
    },

    getItem: function (id) {
        return this._data.hasOwnProperty(id) ? this._data[id] : undefined;
    },

    removeItem: function (id) {
        return delete this._data[id];
    },

    clear: function () {
        return this._data = {};
    }
};

function LocalStorageManager() {
    this.bestScoreKey = "bestScore";
    this.gameStateKey = "gameState";

    var supported = this.localStorageSupported();
    this.storage = supported ? window.localStorage : window.fakeStorage;
}

LocalStorageManager.prototype.localStorageSupported = function () {
    var testKey = "test";

    try {
        var storage = window.localStorage;
        storage.setItem(testKey, "1");
        storage.removeItem(testKey);
        return true;
    } catch (error) {
        return false;
    }
};

// Best score getters/setters
LocalStorageManager.prototype.getBestScore = function () {
    return this.storage.getItem(this.bestScoreKey) || 0;
};

LocalStorageManager.prototype.setBestScore = function (score) {
    this.storage.setItem(this.bestScoreKey, score);
};

// Game state getters/setters and clearing
LocalStorageManager.prototype.getGameState = function () {
    var stateJSON = this.storage.getItem(this.gameStateKey);

    //return stateJSON ? JSON.parse(stateJSON) : null;

    tempGRID = {
        "grid": {
            "size": 4,
            "cells": [[{
                "position": {
                    "x": 0,
                    "y": 0
                },
                "value": 2048
            }, {
                "position": {
                    "x": 0,
                    "y": 1
                },
                "value": 16
            }, {
                "position": {
                    "x": 0,
                    "y": 2
                },
                "value": 8
            }, null], [{
                "position": {
                    "x": 1,
                    "y": 0
                },
                "value": 1024
            }, {
                "position": {
                    "x": 1,
                    "y": 1
                },
                "value": 32
            }, {
                "position": {
                    "x": 1,
                    "y": 2
                },
                "value": 4
            }, null], [{
                "position": {
                    "x": 2,
                    "y": 0
                },
                "value": 512
            }, {
                "position": {
                    "x": 2,
                    "y": 1
                },
                "value": 64
            }, {
                "position": {
                    "x": 2,
                    "y": 2
                },
                "value": 2
            }, null], [{
                "position": {
                    "x": 3,
                    "y": 0
                },
                "value": 256
            }, {
                "position": {
                    "x": 3,
                    "y": 1
                },
                "value": 128
            }, {
                "position": {
                    "x": 3,
                    "y": 2
                },
                "value": 2
            }, null]]
        },
        "score": 4,
        "over": false,
        "won": false,
        "keepPlaying": false
    }
    return tempGRID;
};

LocalStorageManager.prototype.setGameState = function (gameState) {
    this.storage.setItem(this.gameStateKey, JSON.stringify(gameState));
};

LocalStorageManager.prototype.clearGameState = function () {
    this.storage.removeItem(this.gameStateKey);
};