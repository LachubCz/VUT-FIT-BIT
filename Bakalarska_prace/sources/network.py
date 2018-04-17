"""
docstring
"""
from keras.models import Model
from keras.layers import Input, Conv2D, Flatten, Dense, Concatenate, Lambda, Subtract, Add
from keras import backend as K
from keras import optimizers, losses

class Network:
    """
    docstring
    """
    def make_basic_model(state_size, action_size, learning_rate):
        """
        docstring
        """
        network_input = Input(shape=(state_size,))

        net = Dense(units=32, activation="relu", kernel_initializer="he_uniform")(network_input)
        net = Dense(units=16, activation="relu", kernel_initializer="he_uniform")(net)
        net = Dense(units=action_size, activation="linear", kernel_initializer="he_uniform")(net)

        model = Model(inputs=network_input, outputs=net)

        model.summary()
        #plot_model(model, to_file='model.pdf', show_shapes=True, show_layer_names=False)

        model.compile(loss=losses.mean_squared_error, optimizer=optimizers.Adam(lr=learning_rate), metrics=['accuracy'])

        return model

    def make_basic_img_model(state_size, action_size, learning_rate):
        """
        docstring
        """
        network_input = Input(shape=(state_size))

        net = Conv2D(filters=32, kernel_size=(8, 8), strides=(4, 4), activation="relu", 
                     kernel_initializer="he_uniform", data_format="channels_first")(network_input)
        net = Conv2D(filters=64, kernel_size=(4, 4), strides=(2, 2), activation="relu",
                     kernel_initializer="he_uniform")(net)
        net = Conv2D(filters=64, kernel_size=(3, 3), strides=(1, 1), activation="relu",
                     kernel_initializer="he_uniform")(net)
        net = Flatten()(net)

        net = Dense(units=512, activation="relu", kernel_initializer="he_uniform")(net)
        net = Dense(units=action_size, activation="linear", kernel_initializer="he_uniform")(net)

        model = Model(inputs=network_input, outputs=net)

        model.summary()
        plot_model(model, to_file='model.pdf', show_shapes=True, show_layer_names=False)

        model.compile(loss=losses.mean_squared_error, optimizer=optimizers.Adam(lr=learning_rate), metrics=['accuracy'])

        return model

    def make_dueling_model(state_size, action_size, learning_rate):
        """
        docstring
        """
        network_input = Input(shape=(state_size,))

        net = Dense(units=32, activation="relu", kernel_initializer="he_uniform")(network_input)
        net = Dense(units=16, activation="relu", kernel_initializer="he_uniform")(net)

        state_value = Dense(units=1, activation="linear", kernel_initializer="he_uniform")(net)
        value_function = Concatenate(axis=-1)([state_value, state_value])

        action_values = Dense(units=action_size, activation="linear", kernel_initializer="he_uniform")(net)
        avg_action = Lambda(lambda x: K.mean(x, axis=1, keepdims=True))(action_values)
        concat_avg_action = Concatenate(axis=-1)([avg_action, avg_action])

        for i in range(action_size-2):
            value_function = Concatenate(axis=-1)([value_function, state_value])
            concat_avg_action = Concatenate(axis=-1)([concat_avg_action, avg_action])

        advantage_function = Subtract()([action_values, concat_avg_action])

        net = Add()([value_function, advantage_function])

        model = Model(inputs=network_input, outputs=net)

        model.summary()
        #plot_model(model, to_file='model.pdf', show_shapes=True, show_layer_names=False)

        model.compile(loss=losses.mean_squared_error, optimizer=optimizers.Adam(lr=learning_rate), metrics=["accuracy"])

        return model

    def make_dueling_img_model(state_size, action_size, learning_rate):
        """
        docstring
        """
        network_input = Input(shape=(state_size))

        net = Conv2D(filters=32, kernel_size=(8, 8), strides=(4, 4), activation="relu", 
                     kernel_initializer="he_uniform", data_format="channels_first")(network_input)
        net = Conv2D(filters=64, kernel_size=(4, 4), strides=(2, 2), activation="relu",
                     kernel_initializer="he_uniform")(net)
        net = Conv2D(filters=64, kernel_size=(3, 3), strides=(1, 1), activation="relu",
                     kernel_initializer="he_uniform")(net)
        net = Flatten()(net)

        net = Dense(units=512, activation="relu", kernel_initializer="he_uniform")(net)

        state_value = Dense(units=1, activation="linear", kernel_initializer="he_uniform")(net)
        value_function = Concatenate(axis=-1)([state_value, state_value])

        action_values = Dense(units=action_size, activation="linear", kernel_initializer="he_uniform")(net)
        avg_action = Lambda(lambda x: K.mean(x, axis=1, keepdims=True))(action_values)
        concat_avg_action = Concatenate(axis=-1)([avg_action, avg_action])

        for i in range(action_size-2):
            value_function = Concatenate(axis=-1)([value_function, state_value])
            concat_avg_action = Concatenate(axis=-1)([concat_avg_action, avg_action])

        advantage_function = Subtract()([action_values, concat_avg_action])

        net = Add()([value_function, advantage_function])

        model = Model(inputs=network_input, outputs=net)

        model.summary()
        #plot_model(model, to_file='model.pdf', show_shapes=True, show_layer_names=False)

        model.compile(loss=losses.mean_squared_error, optimizer=optimizers.Adam(lr=learning_rate), metrics=['accuracy'])

        return model
