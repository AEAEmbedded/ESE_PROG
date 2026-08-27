# Choosing a Development Method

> *Application Services the way we see it*
>
> *in collaboration with —* **Insert partner logo**

---

## Authors

**Jan Klabbers** — an experienced requirements specialist and software development process engineer at Capgemini, with most of his experience in custom software development. Active in the public sector, public transport, healthcare, manufacturing and insurance. Within Capgemini he leads the community of requirements specialists to move toward a more standardized way of working.

**Erik Hoolt** — project manager at Capgemini with more than ten years of project management experience and over fifteen years in custom software development. He has delivered many projects using various development approaches such as Waterfall, RUP and Scrum. Active for clients in the financial sector, government and the private sector. Offshore collaboration (e.g., with India) is one of his specialties, and he continuously optimizes the ways of working within projects.

---

## Foreword

The immediate reason for writing the first version of this whitepaper was a client request: provide a concise guideline for choosing a suitable development method. And preferably tomorrow rather than next week.

This article primarily reflects the practical experience of two Capgemini consultants with extensive backgrounds in custom software projects. Their views were reviewed by colleagues, adding even more practical experience and theoretical knowledge to the current version. Judging by the reactions, this is a topic that resonates widely.

This document captures a discussion that gives us a handbook for deciding which development method to apply to a project.

An important observation underlying this whitepaper is that companies often too readily embrace a single development method and then try to force‑fit it to all their projects—often resulting in problems and resistance in the organization. Our starting point—fed by Capgemini’s Technovision 2011—is that every development method has strengths and weaknesses, and that certain factors make a method more or less suitable for a project.

Assessing the criteria mentioned requires hands‑on experience. Only by probing certain topics will the necessary information surface. This whitepaper and its accompanying tool provide a foundation for experts to work with the client to determine which method fits best.

---

## Contents

1. [Executive Summary](#executive-summary)
2. [Introduction](#introduction)
3. [Development Methods](#development-methods)
4. [Aspects and Criteria](#aspects-and-criteria)
   - [Client properties](#client-properties)
   - [Supplier properties](#supplier-properties)
   - [Contract](#contract)
   - [Product aspects](#product-aspects)

---

## Executive Summary

Choosing a development method that fits the client, the supplier, the contract type and the product to be delivered is a complex decision. Based on practical experience, this whitepaper offers a guide to help make that choice.

We distinguish three main groups, each with characteristic traits:

- **Linear** — no repetition of process steps.
- **Iterative** — a predefined set of repetitions.
- **Agile** — a predefined set of short iterations.

Hybrid forms are possible; imagine a scale with *linear* on the left, *iterative* in the middle and *agile* on the right. Every project can be plotted somewhere on this scale.

**Assessment per aspect**

Depending on the properties of the client, supplier, contract and product, we advise on the applicability of each group of development methods:

- **A** — a prerequisite for a successful project.
- **B** — increases the likelihood of success.
- **C** — no influence on success.
- **D** — limited measures required to keep the chance of success up.
- **E** — extensive measures required to keep the chance of success up.

**Example** — low stakeholder availability (e.g., the product owner in Scrum):

| Method   | Score | Meaning |
|---|---|---|
| Linear   | D | Limited measures needed to keep the project on track. |
| Iterative| D | Limited measures needed to keep the project on track. |
| Agile    | E | Extensive measures required to keep the project on track. |

With low availability this risk must be managed; for an agile approach, deliberate measures are often required (e.g., a hybrid between agile and iterative). Naturally, all criteria must be considered.

> A suitable approach scores many **A/B/C**, few **D**, and preferably no **E**. Properties with **D/E** must be managed as risks; **E** scores often have large impact and high probability, and often lead to a hybrid approach.

---

## Introduction

In the world of custom software, many development methods are available (LAD, IAD, DSDM, RUP, Scrum, Smart, etc.). The choice is often influenced by the preferences of the client, the supplier, the project manager, or by trends in the industry.

In this document we collect Capgemini’s best practices to arrive at a more substantiated choice based on the properties of:

- **the client organization**
- **the supplier/contractor**
- **the contract type**
- **the product to be delivered**

Based on these criteria, a tool was created that graphically shows the suitability of methods.

> *Example charts (not included in this Markdown).*  
> *Area scores per method type* and *Total score per method type*.

**Structure of this document:**

1. Definition of the three groups (linear, iterative, agile).
2. Criteria for applying a development method.

---

## Development Methods

Many development methods exist. We distinguish three main types: **linear**, **iterative** and **agile**. The distinction between iterative and agile matters because the success factors differ. Hybrid forms are possible; projects may move along the *linear—iterative—agile* scale depending on circumstances.

**Example:** In an agile project it turns out the product owner lacks decision mandate (e.g., due to company culture). A weekly stakeholder board can be set up to make decisions. This reduces agility and shifts the project towards iterative.

### Characteristics per main type

**Linear**

- No repetition of steps in the development process.
- Fixed deliverable at the end of each phase (deliverable‑driven).
- Fixed requirements; documentation up front.
- Document‑driven knowledge transfer.
- High client involvement early/on the front end; later focused on review/approval.
- Limited attention to eliminating technology risks.
- “According to specification” approach.
- Manager‑led teams; fixed roles with their own deliverables.

**Iterative**

- In the first phase: define the set of iterations and their content; evaluate and adjust per iteration.
- Early in the trajectory define ~80% of the requirements; changes via a change process.
- Documentation during development; document‑driven plus in‑person sessions.
- Regular client involvement.
- Mitigating (technology) risks as a driver.
- Combination of “according to specification” and “business value”.
- Manager‑led teams; fixed roles with their own deliverables.

**Agile**

- Short sprints (max. 1 month); sprint scope set at the start of the sprint.
- Daily opportunity to adjust ways of working.
- Ordered product backlog; continuously updated based on client/market needs.
- Highest priority: deliver maximum current business value.
- Documentation: only what’s needed; emphasis on collaboration and verbal transfer.
- “Business value” approach.
- Multidisciplinary, self‑managing teams; responsibility for delivered products.
- High client involvement (product owner).

> Techniques (Use Cases, UML, prototyping, daily stand‑ups, etc.) are not method‑defining; they can be valuable in multiple methods.

---

## Aspects and Criteria

Choosing a development method depends on properties of the **client**, **supplier**, **contract** and **product**. Each property affects success as follows:

- **A** — prerequisite for success
- **B** — increases the chance of success
- **C** — no influence
- **D** — limited measures required
- **E** — extensive measures required

A suitable approach scores many **A/B/C**, few **D**, and preferably no **E**. **D/E** are risks and often lead to a hybrid approach.

### Client properties

#### 4.1.1 Stakeholder availability

Required participation differs by method: linear demands short, intense involvement at specific moments; agile requires continuous participation by the product owner. If high, continuous availability isn’t feasible, choose a more iterative approach with agile techniques.

| Availability | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — a few days per month | D | D | E |
| Medium — one day per week | D | B | D |
| High — hours daily | C | B | A |

#### 4.1.2 Delegation culture

If decision‑making authority is not at a low level in the organization (or is informally taken higher up), making daily decisions is difficult.

| Decision level | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Top management | B | E | E |
| Middle management | C | B | E |
| Mandated to project members | C | B | A |

Mitigation for **E**: organize, for example, a weekly decision‑making meeting (reduces agility).

#### 4.1.3 Decision‑making ability

Does the culture favor quick decisions or broad alignment across the organization?

| Decision‑making ability | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — alignment across the organization | B | D | E |
| Medium — at department level | C | B | E |
| High — staff take decisions | C | B | A |

Mitigation: weekly stakeholder meeting.

#### 4.1.4 Stability of scope and requirements

Does the business know in advance what is needed?

| Stability | Linear | Iterative | Agile |
|---|---:|---:|---:|
| High — 100% | A | B | C |
| Medium — 80% known | D | B | C |
| Low — end result open | E | D | B |

#### 4.1.5 Dealing with uncertainty

Can the organization cope with uncertainty in outcomes? A product vision that bounds the room for change helps.

| Dealing with uncertainty | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Aim for 100% certainty | B | D | E |
| Can live with ~80% | C | B | E |
| Less is also fine | D | B | A |

#### 4.1.6 Possible deployment frequency

How often can new versions be rolled out (test/acceptance/production)? The project team can set up its own environments; low frequency shifts the approach toward iterative or linear.

| Deployment frequency | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — at most once per quarter | B | B | E |
| Medium — monthly | C | B | B |
| High — weekly or more often | C | C | A |

#### 4.1.7 Business maturity in IT

How experienced is the business in IT projects?

| Experience in projects | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — only a few projects to date | D | B | D |
| Medium — one or two per year | B | B | D |
| High — multiple in parallel per year | D | C | A |

> Note: linear is negatively affected by limited IT experience; a full design up‑front demands a lot of imagination.

### Supplier properties

#### 4.2.1 Team seniority

Teams are a mix of seniority; team size influences the use of junior members.

| Seniority | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — many juniors/mediors | C | D | E |
| Medium — mixed with coaching | C | B | C |
| High — hardly any juniors | C | B | A |

#### 4.2.2 Multidisciplinary staff

Multidisciplinarity increases speed and flexibility; essential in agile.

| Multidisciplinary team members | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Low — ~10% | B | D | E |
| Medium — 25–50% | C | B | D |
| High — >75% | C | C | A |

### Contract

#### 4.3.1 Expectation of results

How will results be assessed?

| Result expectation | Linear | Iterative | Agile |
|---|---:|---:|---:|
| In line with pre‑approved specifications | A | C | D |
| In line with specifications managed during the project | D | B | D |
| Open within broad scope, time & budget constraints | E | D | B |

#### 4.3.2 Billing methods

Contractual settlement forms:

| Billing method | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Fixed — after sign‑off on requirements | B | B | D |
| Fixed — after sign‑off + risk coverage | B | B | E |
| Fixed — budget box | C | B | B |
| Time & Material | B | B | C |

> Mitigation for agile in case of mismatch: discuss a contract form that aligns better and delivers more value.

#### 4.3.3 Agreements on delivery date

| Setting the delivery date | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Open | B | C | D |
| After sign‑off on requirements | B | D | D |
| After sign‑off + risk coverage | B | B | D |
| Time box | D | B | A |

#### 4.3.4 Functional scope in the contract

May differ from actual requirements stability.

| Functional scope | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Fixed | A | B | D |
| Medium — ~80% known | D | B | D |
| Open | E | D | B |

### Product aspects

#### 4.4.1 Type of added value

| Added value | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Compliance | B | B | C |
| Mix of compliance and business value | D | B | C |
| Business value | D | D | B |

#### 4.4.2 Familiarity with technology

| Technology | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Proven | A | C | B |
| New — explicit goal of the project | E | B | C |
| New — not a goal of the project | E | B | D |

Working with new technology in agile is possible if *proving the technology* is the explicit goal (with its own product owner). The ultimate product project can then be run separately with another product owner.

#### 4.4.3 Time‑to‑market

| Time‑to‑market | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Long — ~1 year | A | B | D |
| Medium — ~1 quarter | D | B | C |
| High — < 1 month | E | E | B |

#### 4.4.4 Release cycle

| Release cycle | Linear | Iterative | Agile |
|---|---:|---:|---:|
| Year | A | B | C |
| Quarter | D | B | C |
| Month or less | E | E | B |

---

## About Capgemini

With more than 120,000 people in 40 countries, Capgemini is one of the world’s foremost providers of consulting, technology and outsourcing services. In 2011 Capgemini Group reported revenues of €9.7 billion. Together with its clients, Capgemini creates and delivers business and technology solutions that fit client needs and deliver results. As a culturally diverse organization, Capgemini has its own distinctive way of working, the *Collaborative Business Experience™*. Capgemini uses the global delivery model Rightshore®. More information: **www.nl.capgemini.com**.

Rightshore® is a trademark of Capgemini.

**Capgemini Nederland B.V.**  
Papendorpseweg 100  
PO Box 2575 — 3500 GN Utrecht  
[jan.klabbers@capgemini.com](mailto:jan.klabbers@capgemini.com) — Tel +31 30 689 84 54  
[erik.hoolt@capgemini.com](mailto:erik.hoolt@capgemini.com) — Tel +31 30 689 53 44

---

> *Address blocks and stock credits from the PDF are not included. Logos/imagery and charts are omitted in this Markdown.*
