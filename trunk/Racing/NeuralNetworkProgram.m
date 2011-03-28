clear
clc

% Neural network parameters
numNeurons = 20;
numInputs = 14;
numOutputs = 1;

% Get data from text file
data = load('data.dat', '-ascii');

% Parse data file into input and output vectors
input = data(:,1:numInputs)';
output = data(:,numInputs+1)';

% Make the neural network
net = newff(input,output,numNeurons,{'logsig','purelin'},'trainlm','learngdm','mse',{},{});
net.trainParam.max_fail = 100;
net.trainParam.epochs = 50;

% Train the neural network
net = train(net,input,output);

% Get a vector of weight and bias values for network
weightAndBias = getx(net);

% Assign weight and bias for inputs to layers
weight_inputToLayers = weightAndBias(1:numInputs * numNeurons);
bias_inputToLayers = weightAndBias(numInputs * numNeurons + 1:numInputs * numNeurons + 1 + (numNeurons-1));

% Assign weight and bias from layers to output
weight_layersToOutput = weightAndBias(numInputs * numNeurons + 1 + (numNeurons-1) + 1:numInputs * numNeurons + 1 + (numNeurons-1) + 1 + (numNeurons-1));
bias_layersToOutput = weightAndBias(end);

% Change order of weight and bias vector to be compatible with Flood
neuronParameter = 1;
neuronIndex = 1;
weightIndex = 1;
% Order hidden layer weights and biases
for i = 1:(numInputs*numNeurons + numNeurons)
    if (neuronParameter == 1)
        weightAndBias(i) = bias_inputToLayers(neuronIndex);
        neuronParameter = neuronParameter + 1;
    elseif (neuronParameter <= numInputs + 1)
        weightAndBias(i) = weight_inputToLayers(weightIndex);
        weightIndex = weightIndex + numNeurons;
        neuronParameter = neuronParameter + 1;
    end
    
    if (neuronParameter > numInputs + 1)
        neuronParameter = 1;
        neuronIndex = neuronIndex + 1;
        weightIndex = neuronIndex;
    end
end
% Order output layer weights and biases
i = i + 1;
weightAndBias(i) = bias_layersToOutput;
i = i + 1;
for j = 1:numNeurons
    weightAndBias(i) = weight_layersToOutput(j);
    i = i + 1;
end

% Write weights and biases to file
fileID = fopen('net.dat','w');
fprintf(fileID,'%f\n',weightAndBias);
fclose(fileID);

% Test values
% compareForces = zeros(size(output,2),numInputs+2);
% for i = 1:size(output,2)
%     compareForces(i,1:numInputs) = input(:,i);
%     compareForces(i,numInputs+1) = sim(net,input(:,i));
%     compareForces(i,numInputs+2) = output(i);
% end