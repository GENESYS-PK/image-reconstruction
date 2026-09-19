#pragma once
#include "pch.hpp"

enum class Primitive
{
	NOT_SET, CIRCLE, ELLIPSE, TRIANGLE
};

enum class Selection
{
	NOT_SET, TRUNCATION, TOURNAMENT
};

enum class Crossover
{
	NOT_SET, UNIFORM, AVERAGE, BLXA
};

enum class Mutation
{
	NOT_SET, UNIFORM, BOUNDARY
};

struct Circle
{
	double x;
	double y;
	double r;
};

struct Ellipse
{
	double x;
	double y;
	double r1;
	double r2;
};

struct Triangle
{
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
};

union Shape
{
	Circle circle;
	Ellipse ellipse;
	Triangle triangle;
};

struct Chromosome
{
	union
	{
		Shape m_shape;
		double m_params[sizeof(Shape) / sizeof(double)];
	};

	BLRgba32 m_color;
};

struct Individual
{
	double m_loss;
	BLImage m_image;
	std::vector<Chromosome> m_chromosomes;

	void render_and_evaluate(BLContext& context, BLImage& reference,
		Primitive primitive, double rscale, double gscale, double bscale);
};

struct Evolution
{
	BLContext& m_context;
	BLImage& m_reference;

	int m_chromosomeCount;
	int m_populationSize;
	int m_elitarism;

	double m_survivalRate;
	double m_mutationProb;

	Individual* m_bestIndividual = nullptr;

	std::mt19937_64 m_prng;
	std::pair<Chromosome, Chromosome> m_domain;
	std::vector<Individual> m_population;
	std::vector<Individual> m_selected;

	Primitive m_primitive = Primitive::NOT_SET;
	Selection m_selection = Selection::NOT_SET;
	Crossover m_crossover = Crossover::NOT_SET;
	Mutation m_mutation = Mutation::NOT_SET;

	Evolution(BLContext& context, BLImage& reference);

	void perform_evaluation();
	void perform_selection();
	void perform_crossover();
	void perform_mutation();

	void reset();
	void simulate_epoch();
};
